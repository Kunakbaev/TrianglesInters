from pathlib import Path
import sys

# Get the parent directory of the current script's parent
parent_dir = Path(__file__).parent.parent
sys.path.insert(0, str(parent_dir))

import common

"""

For this test with choose "Box" around coordinates origin with some fixed side
Then we generate 3 points: on the left, top and right sides of this box. They form new triangle.
This way we can create huge cluster of intersecting triangles. Moreover, we know the answer for this test,
as all triangles are intersecting. Also, on this kind of tests BVH solution should perform poorly and
perform much more slower than on usual test cases.

"""


TEST_DATA_DIR   = common.SCRIPT_DIR + "/tests_data/in_one_plane/"
NUM_TESTS       = 10
NUM_TRIANGLES   = 100000


def generate_triangles():
  triangles = []
  # NUM_TRIANGLES      = random.randint(5, 100)
  HALF_BOX_SIDE_LEN = common.random_double(1e-3, 1e3)
  BOX_SIDE_LEN      = 2 * HALF_BOX_SIDE_LEN
  X_L               = -HALF_BOX_SIDE_LEN
  X_R               = HALF_BOX_SIDE_LEN
  Y_T               = HALF_BOX_SIDE_LEN
  Y_B               = -HALF_BOX_SIDE_LEN

  for _ in range(NUM_TRIANGLES):
    point_a = common.Point(common.random_double(X_L, X_R), common.random_double(Y_B, Y_T), 0)
    point_b = common.Point(common.random_double(X_L, X_R), common.random_double(Y_B, Y_T), 0)
    point_c = common.Point(common.random_double(X_L, X_R), common.random_double(Y_B, Y_T), 0)
    triangles.append(common.Triangle(point_a, point_b, point_c))

  return triangles

if __name__ == "__main__":
  if len(sys.argv) != 3:
    print("Error: this script expects 2 arguments: number of triangles and destination folder")
    exit(1)

  NUM_TRIANGLES = int(sys.argv[1])
  res_folder_path = sys.argv[2]
  TEST_DATA_DIR += res_folder_path + "/"

  common.generate_tests(TEST_DATA_DIR, NUM_TESTS, generate_triangles)
