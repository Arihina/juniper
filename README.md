python3.11 -m venv venv
source venv/bin/activate
pip install -e .

chmod +x run_valgrind.sh
./run_valgrind.sh

pip uninstall juniper -y
pip install -e . --force-reinstall

tree -I "__pycache__|*.pyc|.git|__pycache__|.venv|venv" -L 3

rm -rf build *.egg-info src/juniper/*.so
