import os
import pathlib
import random
import shutil


SCRIPT_DIR      = str(pathlib.Path(__file__).parent.parent)
COORD_PRECISION = 4

class Point:
  def __init__(self, x, y, z):
    self.x_ = x
    self.y_ = y
    self.z_ = z

  def print2file(self, file):
    print(f"{self.x_} {self.y_} {self.z_}", file=file)


class Triangle:
  def __init__(self, a, b, c):
    self.a_ = a
    self.b_ = b
    self.c_ = c

  def print2file(self, file):
    self.a_.print2file(file)
    self.b_.print2file(file)
    self.c_.print2file(file)
    print("", file=file)
  

def save2file(filename, triangles):
  with open(filename, "w") as file:
    print(len(triangles), file=file)
    for triangle in triangles:
      triangle.print2file(file)


def reset_folder(folder_path):
  """One-liner to reset folder"""
  shutil.rmtree(folder_path, ignore_errors=True)
  os.makedirs(folder_path, exist_ok=True)


def random_double(low, high):
  return round(random.uniform(low, high), COORD_PRECISION)


def generate_tests(TEST_DATA_DIR, NUM_TESTS, gen_triangles_function):
  random.seed(228)
  print(TEST_DATA_DIR)
  reset_folder(TEST_DATA_DIR)
  for test_ind in range(NUM_TESTS):
    triangles = gen_triangles_function()
    filename = TEST_DATA_DIR + str(test_ind) + ".dat"
    save2file(filename, triangles)

