"""Verify snapshot and touch translation for the Mac layout preview."""

import json
import unittest

from qt_layout_preview_bridge import QtLayoutPreviewBridge


class QtLayoutPreviewBridgeTest(unittest.TestCase):
    def test_legacy_layout_translates_snapshots_and_touches(self):
        bridge = QtLayoutPreviewBridge()
        state = {"controls": [
            {"label": "FLASH", "x": 540, "y": 184, "w": 111, "h": 84},
            {"label": "AUTO", "x": 16, "y": 56, "w": 68, "h": 96},
        ]}
        rewritten = json.loads(bridge.state_line("@STATE " + json.dumps(state) + "\n")[7:])
        self.assertEqual([rewritten["controls"][0][key] for key in ("x", "y", "w", "h")],
                         [552, 184, 103, 78])
        self.assertEqual(rewritten["controls"][1], state["controls"][1])
        self.assertEqual(bridge.touch_line("D 0 610 210 123\n"), "D 0 602 212 123\n")
        self.assertEqual(bridge.touch_line("D 0 543 210 123\n"), "D 0 -1 -1 123\n")
        self.assertEqual(bridge.touch_line("D 0 66 90 123\n"), "D 0 66 90 123\n")

    def test_current_layout_passes_through(self):
        bridge = QtLayoutPreviewBridge()
        state = {"controls": [{"label": "FLASH", "x": 552, "y": 184, "w": 103, "h": 78}]}
        bridge.state_line("@STATE " + json.dumps(state) + "\n")
        self.assertFalse(bridge.legacy_layout)
        self.assertEqual(bridge.touch_line("D 0 610 210 123\n"), "D 0 610 210 123\n")

    def test_each_effect_stays_inside_its_original_touch_rectangle(self):
        bridge = QtLayoutPreviewBridge()
        self.assertEqual(len(bridge.rectangles), 7)
        controls = []
        for label, (old, _) in bridge.rectangles.items():
            controls.append({"label": label, "x": old[0], "y": old[1],
                             "w": old[2], "h": old[3]})
        state = {"controls": controls}
        rewritten = json.loads(bridge.state_line("@STATE " + json.dumps(state) + "\n")[7:])
        for control, (old, new) in zip(rewritten["controls"], bridge.rectangles.values()):
            with self.subTest(snapshot=control["label"]):
                self.assertEqual(tuple(control[key] for key in ("x", "y", "w", "h")), new)
            x, y = new[0] + new[2] // 2, new[1] + new[3] // 2
            translated = bridge.touch_line(f"D 0 {x} {y} 123\n").split()
            old_x, old_y = map(int, translated[2:4])
            with self.subTest(rectangle=new):
                self.assertLessEqual(old[0], old_x)
                self.assertLess(old_x, old[0] + old[2])
                self.assertLessEqual(old[1], old_y)
                self.assertLess(old_y, old[1] + old[3])

    def test_effect_rows_have_even_gaps(self):
        rectangles = [pair[1] for pair in QtLayoutPreviewBridge.rectangles.values()]
        for first, second in zip(rectangles[:3], rectangles[3:6]):
            self.assertEqual(second[1] - (first[1] + first[3]), 8)
        self.assertEqual(rectangles[6][1] - (rectangles[3][1] + rectangles[3][3]), 8)
        self.assertEqual(rectangles[6][1] - (rectangles[5][1] + rectangles[5][3]), 8)


if __name__ == "__main__":
    unittest.main()
