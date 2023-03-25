import glob
import os
import sys
import pytest

BASE_DIR = os.environ.get("BASE_DIR", ".")
EMUL_HTTP_ME = os.path.join(BASE_DIR, "httpParser")
EMUL_HTTP_PROF = os.path.join(BASE_DIR, "../httpparser")
ALL_FILES = glob.glob(os.path.join(BASE_DIR, "../../Tests/premier-jeu-test/get*[!'.out''.me']"))
ALL_FILES.sort()


class TestHTTP:
    @pytest.mark.parametrize("filename", ALL_FILES)
    def testFiles(self, filename):
        name = os.path.splitext(filename)[0]

        os.system(f"./{EMUL_HTTP_ME} {filename} > {name}.me")
        os.system(f"./{EMUL_HTTP_PROF} {filename} > {name}.out")
        my_resultat = open(f"{name}.me", "r")
        prof_resultat = open(f"{name}.out", "r")
        a = my_resultat.read().strip('\n').split('\n')
        b = prof_resultat.read().strip('\n').split('\n')
        if a == '':
            pytest.fail(f"Sortie vide")

        for i in range(1, min(len(a), len(b))):
            if a[i] != b[i]:
                pytest.fail(f"Error ligne {i}")


pytest.main(sys.argv)
