python3.11 -m venv venv
source venv/bin/activate
pip install -e .

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log python3.11 examples/list.py
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log python3.11 examples/hashmap.py

pip uninstall juniper
pip install -e . --force-reinstall
