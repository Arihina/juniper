import inspect
import juniper


def signature(obj):
    try:
        return str(inspect.signature(obj))
    except Exception:
        return "()"


def get_methods(cls):
    methods = []
    for name, obj in inspect.getmembers(cls):
        if name.startswith("_"):
            continue
        if inspect.isroutine(obj):
            doc = inspect.getdoc(obj) or ""
            first_line = doc.split("\n")[0]
            methods.append((name, signature(obj), first_line))
    return methods


def generate():
    lines = []
    lines.append("# Juniper Data Structures API\n")

    for name in juniper.__all__:
        cls = getattr(juniper, name)

        lines.append(f"\n## {name}\n")
        lines.append("| Method | Signature | Description |")
        lines.append("|---|---|---|")

        for m, sig, doc in get_methods(cls):
            lines.append(f"| `{m}` | `{sig}` | {doc} |")

    return "\n".join(lines)


if __name__ == "__main__":
    with open("API.md", "w", encoding="utf-8") as f:
        f.write(generate())

    print("API.md generated")
    