import glob
import os
import sys
import pytest

BASE_DIR = os.environ.get("BASE_DIR", ".")
EMUL_HTTP = os.path.join(BASE_DIR, "httpParser")
EMUL_HTTP_PROF = os.path.join(BASE_DIR, "testsimplegrammar")
TEST_FILES = os.path.join(BASE_DIR, "../Tests/testgrammar/testFile/")
ALL_FILES = glob.glob(os.path.join(TEST_FILES, "*.txt"))
ALL_FILES.sort()


class TestHTTP:
    @pytest.mark.parametrize("filename", ALL_FILES)
    def testFiles(self, filename):
        name = os.path.splitext(filename)[0]

        os.system(f"./{EMUL_HTTP} {filename} > {name}.me")
        os.system(f"./{EMUL_HTTP_PROF} {filename} > {name}.prof")
        my_resultat = open(f"{name}.me", "r")
        prof_resultat = open(f"{name}.prof", "r")
        a = my_resultat.read()
        b = prof_resultat.read()
        if a == '':
            pytest.fail("Sortie vide")
            resultat = 0
        elif len(a) != len(b):
            pytest.fail("Longueurs différentes")
        else:
            for i in range(len(a)):
                if a[i] != b[i]:
                    pytest.fail(f"Error caractère {i} : \"{a[i]}\" à la place de \"{b[i]}\"")
                    break


pytest.main(sys.argv)
