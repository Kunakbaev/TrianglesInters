import sys
import numpy as np
import matplotlib.pyplot as plt
from display_triangles import create_3d_triangles
import mpl_toolkits

if __name__ == "__main__":
  if (len(sys.argv) != 2):
    print("This script takes only one argument: \n" \
    "path to the file with triangles coordinates, that are going to be processed...")
    exit(1)

  path2file = sys.argv[1]
  print("path2file : ", path2file)

  with open(path2file, "r") as file:
    lines_tmp = file.readlines()
    lines = list(filter(lambda line: len(line) > 1, lines_tmp))

    assert(len(lines) > 0)
    num_triangles = int(lines[0])
    triangles = []

    i = 1
    while i < len(lines):
      if len(lines[i]) == 0:
        i += 1
        continue

      values = []
      for j in range(3):
        tmp = list(map(float, lines[i].split()))
        i += 1
        values += tmp
      # if len(values) == 0:
      #   i += 3
      #   continue
      
      triangle = np.array(values).reshape(3, 3)
      print("triangle : ", triangle)
      triangles.append(triangle)

      num_triangles -= 1
      if num_triangles == 0:
        break

    fig, ax = create_3d_triangles(triangles)
    plt.show()

