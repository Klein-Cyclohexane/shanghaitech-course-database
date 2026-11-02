import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import generate_catalog as generator


class CatalogTests(unittest.TestCase):
    def test_filesystem_and_tree_inputs_publish_separate_identities_and_all_files(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            metadata = {"电路基础": {"course_code": "EE111", "course_type": "theory"},
                        "电路基础实验": {"course_code": "EE111L", "course_type": "lab"}}
            (root / "course_metadata.json").write_text(json.dumps(metadata), encoding="utf-8")
            paths = ["电路基础/EE111 2025Spring.pdf", "电路基础/EE111 Final Cheat_Sheet.pdf",
                     "电路基础实验/Lab1.pdf", "tests/test_generate_catalog.py"]
            for path in paths:
                target = root / path
                target.parent.mkdir(parents=True, exist_ok=True)
                target.touch()
            for source_paths in (None, paths):
                with redirect_stdout(io.StringIO()):
                    catalog = generator.build_catalog(root, root / "catalog.json", paths=source_paths)
                self.assertEqual(catalog["metadata"]["total_files"], 3)
                self.assertNotIn("tests", catalog["courses"])
                for name, identity in metadata.items():
                    for field, value in identity.items():
                        self.assertEqual(catalog["courses"][name][field], value)
                theory = catalog["courses"]["电路基础"]
                self.assertEqual(theory["others"][0]["name"], "EE111 2025Spring.pdf")
                self.assertNotIn("%5C", theory["others"][0]["url"])

    def test_reject_theory_lab_course_code_collision(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "course_metadata.json").write_text(json.dumps({
                "电路基础": {"course_code": "EE111", "course_type": "theory"},
                "电路基础实验": {"course_code": "EE111", "course_type": "lab"}}), encoding="utf-8")
            with self.assertRaises(ValueError):
                generator.build_catalog(root, root / "catalog.json", ["电路基础/a.pdf", "电路基础实验/b.pdf"])

    def test_numeric_lab_code_exceptions_and_legacy_unmapped_courses(self):
        self.assertEqual(generator.course_identity("分子生物学实验", {
            "分子生物学实验": {"course_code": "BIO1024", "course_type": "lab"}})["course_code"], "BIO1024")
        self.assertEqual(generator.course_identity("信号与系统实验", {})["course_type"], "lab")
        self.assertNotIn("course_code", generator.course_identity("未知课程", {}))

    def test_reject_lab_folder_marked_as_theory(self):
        with self.assertRaises(ValueError):
            generator.course_identity("电路基础实验", {"电路基础实验": {"course_type": "theory"}})


if __name__ == "__main__":
    unittest.main()
