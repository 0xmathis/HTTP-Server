import glob
import os
import sys
import pytest

BASE_DIR = os.environ.get("BASE_DIR", ".")
EMUL_HTTP_ME = os.path.join(BASE_DIR, "httpParser")
EMUL_HTTP_PROF = os.path.join(BASE_DIR, "../httpparser")
ALL_FILES = glob.glob(os.path.join(BASE_DIR, "../../Tests/premier-jeu-test/*.txt")) + glob.glob(os.path.join(BASE_DIR, "../../Tests/Only4G33ks/testFile/*.txt"))
ALL_FILES.sort()


class TestHTTP:
    @pytest.mark.parametrize("filename", ALL_FILES)
    def testFiles1(self, filename):
        name = os.path.splitext(filename)[0]

        os.system(f"./{EMUL_HTTP_ME} {filename} > {name}.me")
        os.system(f"./{EMUL_HTTP_PROF} {filename} > {name}.prof")
        my_resultat = open(f"{name}.me", "r")
        prof_resultat = open(f"{name}.prof", "r")
        a = my_resultat.read()
        b = prof_resultat.read()
        if a == '':
            pytest.fail(f"{EMUL_HTTP_ME} : Sortie vide")

        if len(a) != len(b):
            pytest.fail(f"{EMUL_HTTP_ME} : Longueurs différentes")

        for i in range(len(a)):
            if a[i] != b[i]:
                pytest.fail(f"{EMUL_HTTP_ME} :Error caractère {i} : '{a[i]}' à la place de '{b[i]}'")


pytest.main(sys.argv)
