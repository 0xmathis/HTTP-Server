import glob
import os
import sys
import pytest

BASE_DIR = os.environ.get("BASE_DIR", ".")
EMUL_HTTP_ME = os.path.join(BASE_DIR, "httpParser")
EMUL_HTTP_PROF = os.path.join(BASE_DIR, "../httpparser")
ALL_FILES = glob.glob(os.path.join(BASE_DIR, "../../Tests/Only4G33ks/testFile/test??.txt"))
ALL_FILES.sort()


class TestHTTP:
    @pytest.mark.parametrize("filename", ALL_FILES)
    def testFiles1(self, filename):
        name = os.path.splitext(filename)[0]

        os.system(f"./{EMUL_HTTP_ME} {filename} > {name}.me")
        os.system(f"./{EMUL_HTTP_PROF} {filename} > {name}.out")
        my_resultat = open(f"{name}.me", "rb")
        prof_resultat = open(f"{name}.out", "rb")
        a = my_resultat.read().strip(b'\n').split(b'\n')
        b = prof_resultat.read().strip(b'\n')

        if b'IPv6' in b:
            pytest.skip("IPv6 non implémenté par le parseur")

        b = b.split(b'\n')

        if not a:
            pytest.fail("Sortie vide", pytrace=False)

        if len(a) != len(b):
            pytest.fail("Longueurs différentes", pytrace=False)

        for i in range(len(a)):

            if a[i] != b[i]:
                pytest.fail(f"Error ligne {i}", pytrace=False)


pytest.main(sys.argv)
