import math
from pathlib import Path
import sys

# Get the parent directory of the current script's parent
parent_dir = Path(__file__).parent.parent
sys.path.insert(0, str(parent_dir))

import common

"""

In this test case, all triangles are on the same plane (z=0, plane OXY) and each triangle lives in it's own "cell",
i.e. we divide plane into matrix N by N and place triangle in it's own square.
This way we make sure that no intersections will happen. This test fails naive solution
as there's no big cluster of triangles and it has to iterate through every pair.

"""


TEST_DATA_DIR   = common.SCRIPT_DIR + "/tests_data/in_one_plane/"
NUM_TESTS       = 10
EPS             = 1e-3
MESH_SIDE       = 320


def generate_triangles():
  triangles = []
  CELL_SIDE = common.random_double(1e-3, 1e3)

  for i in range(MESH_SIDE):
    for j in range(MESH_SIDE):
      x_l = j * CELL_SIDE + EPS
      x_r = (j + 1) * CELL_SIDE - EPS
      y_t = (i + 1) * CELL_SIDE - EPS
      y_b = i * CELL_SIDE + EPS

      point_a = common.Point(common.random_double(x_l, x_r), common.random_double(y_b, y_t), 0)
      point_b = common.Point(common.random_double(x_l, x_r), common.random_double(y_b, y_t), 0)
      point_c = common.Point(common.random_double(x_l, x_r), common.random_double(y_b, y_t), 0)
      triangles.append(common.Triangle(point_a, point_b, point_c))

  return triangles

if __name__ == "__main__":
  if len(sys.argv) != 3:
    print("Error: this script expects 2 arguments: number of triangles and destination folder")
    exit(1)

  num_triangles = int(sys.argv[1])
  res_folder_path = sys.argv[2]
  TEST_DATA_DIR += res_folder_path + "/"

  print(f"num_triangles : {num_triangles}, res_folder_path: {res_folder_path}\n")
  MESH_SIDE = math.floor(math.sqrt(num_triangles)) + 1
  common.generate_tests(TEST_DATA_DIR, NUM_TESTS, generate_triangles)
