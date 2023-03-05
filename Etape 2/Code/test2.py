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

        out_put = os.system(f"./{EMUL_HTTP} {filename} > {name}.me")
        os.system(f"./{EMUL_HTTP_PROF} {filename} > {name}.prof")
        resultat = 1
        my_resultat = open(f"{name}.me", "r")
        prof_resultat = open(f"{name}.prof", "r")
        a = my_resultat.read()
        b = prof_resultat.read()
        index = -1
        if a == '':
            resultat = 0
        for i in range(0, min(len(a), len(b))):
            if a[i] != b[i]:
                resultat = 0
                index = i
                break

        if resultat == 0:
            pytest.fail(f"Error caractère {index} : \"{a[index]}\" à la place de \"{b[index]}\"")


pytest.main(sys.argv)
