import inspect
import juniper

def format_signature(obj):
    try:
        return str(inspect.signature(obj))
    except ValueError:
        return "()"

def dump_class(cls):
    print(f"\n{cls.__name__}")
    print("=" * len(cls.__name__))

    for name, obj in inspect.getmembers(cls):
        if name.startswith("_"):
            continue
        if inspect.isroutine(obj):
            sig = format_signature(obj)
            doc = inspect.getdoc(obj) or ""
            first_line = doc.split("\n")[0]

            print(f"{name}{sig}")
            if first_line:
                print(f"    {first_line}")

for name in juniper.__all__:
    dump_class(getattr(juniper, name))
