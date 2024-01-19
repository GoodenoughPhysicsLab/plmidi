import platform
import setuptools

if platform.system() == "Windows":
    compile_arg = "/std:c++20"
else:
    compile_arg = "-std=c++20"

setuptools.setup(
    name="plmidi",
    version="1.0.4",
    author="Goodenough",
    author_email="2381642961@qq.com",
    description="midi player for Python Package physicsLab",
    long_description="show detils in [github](https://github.com/GoodenoughPhysicsLab/plmidi)",
    long_description_content_type="text/markdown",
    url="https://github.com/GoodenoughPhysicsLab/plmidi",
    packages=setuptools.find_packages(),
    license="MIT",
    python_requires='>=3.6',
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Natural Language :: Chinese (Simplified)",
        "Operating System :: OS Independent",
    ],
    ext_modules=[
        setuptools.Extension(
            "plmidi_cpp",
            sources=[
                "./plmidi_cpp/plmidi_setup.cpp"
            ],
            extra_compile_args=[
                compile_arg
            ]
        )
    ]
)
