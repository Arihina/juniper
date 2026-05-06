python3.11 -m venv venv
source venv/bin/activate
pip install -e .

chmod +x run_valgrind.sh
./run_valgrind.sh

PYTHONMALLOC=malloc python3.11 memory_runner.py

pip uninstall juniper -y
pip install -e . --force-reinstall

tree -I "__pycache__|*.pyc|.git|__pycache__|.venv|venv" -L 6

rm -rf build *.egg-info src/juniper/*.so
