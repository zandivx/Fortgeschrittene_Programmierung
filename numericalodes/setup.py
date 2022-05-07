from distutils.core import setup, Extension

def main():
    setup(name="numericalodes",
          version="1.0",
          description="TODO",
          author="Andreas Zach",
          author_email="andreas.zach@student.tugraz.at",
          ext_modules=[Extension("numericalodes", ["test_py.c"])])

if __name__ == "__main__":
    main()