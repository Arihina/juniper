from setuptools import setup, Extension, find_packages # type: ignore

native_module = Extension(
    name="junipre._native",
    sources=[
    "src/junipre/cext/module.c",
    "src/junipre/cext/slist_module.c",
    "src/junipre/cext/hashmap_module.c",
    "source/slist.c",
    "source/hashmap.c",
    ],
    include_dirs=["source"],
    extra_compile_args=["-O3"],
)

setup(
    name="juniper",
    version="0.1.0",
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    ext_modules=[native_module], 
)
