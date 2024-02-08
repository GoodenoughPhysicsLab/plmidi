import platform
import setuptools

if platform.system() == "Windows":
    compile_args = ["/std:c++20"]
else:
    compile_args = ["-std=c++20"]

setuptools.setup(
    name="plmidi",
    version="",
    author="Goodenough",
    author_email="2381642961@qq.com",
    description="midi player for Python Package physicsLab",
    long_description="show detils in [github](https://github.com/GoodenoughPhysicsLab/plmidi)",
    long_description_content_type="text/markdown",
    url="https://github.com/GoodenoughPhysicsLab/plmidi",
    packages=setuptools.find_packages(),
    install_requires=["mido"],
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
            name="plmidi_cpp",
            language="c++",
            sources=[
                "plmidi_cpp/setup.cpp"
            ],
            extra_compile_args=compile_args
        )
    ]
)
