#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Generate catalog.json for XiongWenye/shanghaitech-course-database
This catalog acts as a machine-readable index for Agent Skills and third-party tools,
completely decoupling dynamic repository updates from static downstream code.
"""

import os
import sys
import json
import re
import urllib.parse
from pathlib import Path
from datetime import datetime, timezone

BASE_DIR = Path(__file__).resolve().parent.parent
OUTPUT_FILE = BASE_DIR / "catalog.json"

REPO_OWNER = "XiongWenye"
REPO_NAME = "shanghaitech-course-database"
BASE_URL = f"https://github.com/{REPO_OWNER}/{REPO_NAME}/tree/main"
BLOB_URL = f"https://github.com/{REPO_OWNER}/{REPO_NAME}/blob/main"

EXCLUDED_DIRS = {".git", ".github", "scripts", "tests", ".idea", ".vscode", "__pycache__"}
EXCLUDED_FILES = {".DS_Store", "catalog.json", "README.md", ".gitignore", "LICENSE"}


def is_exam_file(path: str) -> bool:
    fl = path.lower()
    clean_fl = re.sub(r'examples?', '', fl)
    if any(k in clean_fl for k in ['final', 'midterm', '期末', '期中', '试卷', '考试', '真题', 'cheatsheet', 'cheat sheet', 'cheat_sheet', '速查', 'quiz', '小测', '测验', 'crib']):
        return True
    if re.search(r'[\W_]exam[\W_.]', clean_fl) or clean_fl.endswith(('exam.pdf', 'exam.docx', 'exam.md')):
        return True
    # Chinese abbreviation: 中 (midterm) / 末 (final)
    # e.g., 24-25-1-中.pdf, 24-25-1-末.pdf, 2023-1中.pdf, 22-23末.pdf, 2020-1中(2).pdf
    if re.search(r'(\d+[-_]|\d+)(中|末)(卷)?(\.|\([0-9]+\)\.|[-_])', clean_fl):
        return True
    return False


def is_solution_file(path: str) -> bool:
    fl = path.lower()
    if any(k in fl for k in ['solution', 'solutions', 'answer', 'answers', '答案', '解析', '简析', '评分标准', 'crib']):
        return True
    if re.search(r'[\W_](sol|ans|key)[\W_.]', fl):
        return True
    if fl.endswith(('_sol.pdf', '-sol.pdf', '_ans.pdf', '-ans.pdf', '_key.pdf', '-key.pdf', '_sol.docx', '-sol.docx')):
        return True
    return False


def course_identity(course_name, overrides):
    """Publish stable identities so clients need not use substring matching."""
    lab = bool(re.search(r"实验(?:课)?$|\blab(?:oratory)?$", course_name, re.I)
               or re.fullmatch(r"[A-Z]+\d+[A-Z]*L", course_name, re.I))
    identity = {"course_type": "lab" if lab else "theory"}
    identity.update(overrides.get(course_name, {}))
    if identity["course_type"] not in ("theory", "lab"):
        raise ValueError(f"Invalid course_type: {course_name}")
    if lab and identity["course_type"] != "lab":
        raise ValueError(f"Lab folder marked as theory: {course_name}")
    code = identity.get("course_code")
    if code and not re.fullmatch(r"[A-Z]+\d+[A-Z]*", code):
        raise ValueError(f"Invalid course_code for {course_name}: {code}")
    if code and code.endswith("L") and identity["course_type"] != "lab":
        raise ValueError(f"Lab code marked as theory: {code}")
    return identity


def repository_paths(repo_dir):
    for entry in sorted(repo_dir.iterdir()):
        if not entry.is_dir() or entry.name in EXCLUDED_DIRS or entry.name.startswith("."):
            continue
        for root, dirs, files in os.walk(entry):
            dirs[:] = sorted(d for d in dirs if not d.startswith(".") and d not in EXCLUDED_DIRS)
            for name in sorted(files):
                yield (Path(root) / name).relative_to(repo_dir).as_posix()


def build_catalog(repo_dir=BASE_DIR, output_file=OUTPUT_FILE, paths=None):
    repo_dir = Path(repo_dir)
    metadata_file = repo_dir / "course_metadata.json"
    overrides = json.loads(metadata_file.read_text(encoding="utf-8")) if metadata_file.exists() else {}
    courses = {}
    total_files = 0
    total_exams = 0
    total_solutions = 0

    ordered_paths = sorted(repository_paths(repo_dir) if paths is None else paths,
                           key=lambda path: (path.split("/")[:-1], path.split("/")[-1]))
    for rel_f in ordered_paths:
        parts = rel_f.split("/")
        if len(parts) < 2 or any(p in EXCLUDED_DIRS or p.startswith(".") for p in parts[:-1]):
            continue
        course_name, f = parts[0], parts[-1]
        if f in EXCLUDED_FILES or f.startswith("."):
            continue
        encoded_course_name = urllib.parse.quote(course_name)
        if course_name not in courses:
            courses[course_name] = {
                **course_identity(course_name, overrides),
                "folder_name": course_name,
                "repo_url": f"{BASE_URL}/{encoded_course_name}",
                "exams": [],
                "solutions": [],
                "slides_notes": [],
                "labs": [],
                "others": [],
                "total_files": 0
            }

        file_info = {"name": f, "rel_path": rel_f,
                     "url": f"{BLOB_URL}/{urllib.parse.quote(rel_f)}"}
        course_data = courses[course_name]
        course_data["total_files"] += 1
        total_files += 1
        fl = f.lower()
        if is_exam_file(rel_f):
            if is_solution_file(rel_f):
                course_data["solutions"].append(file_info)
                total_solutions += 1
            else:
                course_data["exams"].append(file_info)
                total_exams += 1
        elif is_solution_file(rel_f):
            course_data["solutions"].append(file_info)
            total_solutions += 1
        elif any(k in fl for k in ["实验", "lab"]):
            course_data["labs"].append(file_info)
        elif any(k in fl for k in ["讲义", "课件", "ppt", "slide", "第", "章", "note", "笔记", "mp4"]):
            course_data["slides_notes"].append(file_info)
        else:
            course_data["others"].append(file_info)

    code_types = {}
    for name, course in courses.items():
        code = course.get("course_code")
        if code:
            if code in code_types and code_types[code] != course["course_type"]:
                raise ValueError(f"Theory/lab folders share course_code: {code}")
            code_types[code] = course["course_type"]

    catalog = {
        "metadata": {
            "schema_version": "2.0",
            "repo": f"{REPO_OWNER}/{REPO_NAME}",
            "generated_at": datetime.now(timezone.utc).isoformat(),
            "total_courses": len(courses),
            "total_files": total_files,
            "total_exams": total_exams,
            "total_solutions": total_solutions
        },
        "courses": courses
    }

    # Keep existing file order to make identity-only updates reviewable; newly
    # discovered files append in stable path order. Never copy stale file entries.
    output_file = Path(output_file)
    try:
        previous_courses = json.loads(output_file.read_text(encoding="utf-8")).get("courses", {})
    except (OSError, ValueError):
        previous_courses = {}
    for name, course in courses.items():
        for category in ("exams", "solutions", "slides_notes", "labs", "others"):
            previous = previous_courses.get(name, {}).get(category, [])
            order = {file["url"]: position for position, file in enumerate(previous)}
            course[category].sort(key=lambda file: (order.get(file["url"], len(order)), file["rel_path"]))

    with open(output_file, "w", encoding="utf-8") as f:
        json.dump(catalog, f, ensure_ascii=False, indent=2)

    print(f"✅ Generated catalog.json successfully:")
    print(f"   Courses: {len(courses)}")
    print(f"   Total Files: {total_files}")
    print(f"   Exams: {total_exams}, Solutions: {total_solutions}")
    print(f"   Output saved to: {output_file}")
    return catalog


if __name__ == "__main__":
    build_catalog()
