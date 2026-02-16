from setuptools import setup, Extension

native_module = Extension(
    name="junipre._native",
    sources=[
        "src/junipre/cext/bindings.c",
        "source/slist.c",
    ],
    include_dirs=["source"],
    extra_compile_args=["-O3"],
)

setup(
    ext_modules=[native_module],
)
