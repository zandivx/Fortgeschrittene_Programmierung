from setuptools import setup, Extension


def main() -> None:
    setup(
        name="numericalodes",
        version="0.1",
        description="TODO",
        homepage="https://github.com/zandivx/numericalodes",
        author="Andreas Zach",
        author_email="andreas.zach@student.tugraz.at",
        license="MIT License",
        ext_modules=[Extension("numericalodes", ["numericalodes.c"])],
    )


if __name__ == "__main__":
    main()
