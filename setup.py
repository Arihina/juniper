from setuptools import setup, Extension, find_packages
 
native_module = Extension(
    name="juniper._native",
    sources=[
    "src/juniper/cext/module.c",
    "src/juniper/cext/slist_type.c",
    "src/juniper/cext/dlist_type.c",
    "src/juniper/cext/hashmap_type.c",
     "src/juniper/cext/rhmap_type.c",
    "src/juniper/cext/rbset_type.c",
    "source/slist.c",
    "source/dlist.c",
    "source/hashmap.c",
    "source/rhmap.c",
    "source/rbtree.c",
    ],
    include_dirs=["source", "src/juniper/cext"],
    extra_compile_args=["-O3"],
)
 
setup(
    name="juniper",
    version="0.1.0",
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    ext_modules=[native_module],
)
