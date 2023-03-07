import glob
import os
import sys
import pytest

BASE_DIR = os.environ.get("BASE_DIR", ".")
EMUL_HTTP_1 = os.path.join(BASE_DIR, "httpParser")
EMUL_HTTP_2 = os.path.join(BASE_DIR, "rescue")
EMUL_HTTP_PROF = os.path.join(BASE_DIR, "../../Tests/testgrammar/testsimplegrammar")
PROF_TEST_FILES = os.path.join(BASE_DIR, "../../Tests/testgrammar/testFile/")
MY_TEST_FILES = os.path.join(BASE_DIR, "Tests/")
ALL_FILES = glob.glob(os.path.join(PROF_TEST_FILES, "*.txt")) + glob.glob(os.path.join(MY_TEST_FILES, "*.txt"))
ALL_FILES.sort()


class TestHTTP:
    @pytest.mark.parametrize("filename", ALL_FILES)
    def testFiles1(self, filename):
        name = os.path.splitext(filename)[0]

        os.system(f"./{EMUL_HTTP_1} {filename} > {name}.me1")
        os.system(f"./{EMUL_HTTP_PROF} {filename} > {name}.prof")
        my_resultat = open(f"{name}.me1", "r")
        prof_resultat = open(f"{name}.prof", "r")
        a = my_resultat.read()
        b = prof_resultat.read()
        if a == '':
            pytest.fail(f"{EMUL_HTTP_1} : Sortie vide")

        if len(a) != len(b):
            pytest.fail(f"{EMUL_HTTP_1} : Longueurs différentes")

        for i in range(len(a)):
            if a[i] != b[i]:
                pytest.fail(f"{EMUL_HTTP_1} :Error caractère {i} : \"{a[i]}\" à la place de \"{b[i]}\"")

    @pytest.mark.parametrize("filename", ALL_FILES)
    def testFiles2(self, filename):
        name = os.path.splitext(filename)[0]

        os.system(f"./{EMUL_HTTP_2} {filename} > {name}.me2")
        os.system(f"./{EMUL_HTTP_PROF} {filename} > {name}.prof")
        my_resultat = open(f"{name}.me2", "r")
        prof_resultat = open(f"{name}.prof", "r")
        a = my_resultat.read()
        b = prof_resultat.read()
        if a == '':
            pytest.fail(f"{EMUL_HTTP_2} : Sortie vide")

        if len(a) != len(b):
            pytest.fail(f"{EMUL_HTTP_2} : Longueurs différentes")

        for i in range(len(a)):
            if a[i] != b[i]:
                pytest.fail(f"{EMUL_HTTP_2} :Error caractère {i} : \"{a[i]}\" à la place de \"{b[i]}\"")


pytest.main(sys.argv)
