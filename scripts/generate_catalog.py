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

EXCLUDED_DIRS = {".git", ".github", "scripts", ".idea", ".vscode", "__pycache__"}
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


def build_catalog():
    courses = {}
    total_files = 0
    total_exams = 0
    total_solutions = 0

    # Sort directories alphabetically
    for entry in sorted(os.listdir(BASE_DIR)):
        entry_path = BASE_DIR / entry
        if not entry_path.is_dir() or entry in EXCLUDED_DIRS or entry.startswith('.'):
            continue

        course_name = entry
        encoded_course_name = urllib.parse.quote(course_name)

        course_data = {
            "folder_name": course_name,
            "repo_url": f"{BASE_URL}/{encoded_course_name}",
            "exams": [],
            "solutions": [],
            "slides_notes": [],
            "labs": [],
            "others": [],
            "total_files": 0
        }

        for root, dirs, files in os.walk(entry_path):
            if ".git" in root:
                continue
            for f in sorted(files):
                if f in EXCLUDED_FILES or f.startswith('.'):
                    continue

                abs_f = os.path.join(root, f)
                rel_f = os.path.relpath(abs_f, BASE_DIR)
                encoded_rel = urllib.parse.quote(rel_f)
                file_url = f"{BLOB_URL}/{encoded_rel}"

                file_info = {
                    "name": f,
                    "rel_path": rel_f,
                    "url": file_url
                }

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

        courses[course_name] = course_data

    catalog = {
        "metadata": {
            "repo": f"{REPO_OWNER}/{REPO_NAME}",
            "generated_at": datetime.now(timezone.utc).isoformat(),
            "total_courses": len(courses),
            "total_files": total_files,
            "total_exams": total_exams,
            "total_solutions": total_solutions
        },
        "courses": courses
    }

    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        json.dump(catalog, f, ensure_ascii=False, indent=2)

    print(f"✅ Generated catalog.json successfully:")
    print(f"   Courses: {len(courses)}")
    print(f"   Total Files: {total_files}")
    print(f"   Exams: {total_exams}, Solutions: {total_solutions}")
    print(f"   Output saved to: {OUTPUT_FILE}")
    return catalog


if __name__ == "__main__":
    build_catalog()
