import glob
import os
import sys
import pytest
import subprocess

BASE_DIR = os.environ.get("BASE_DIR", ".")
EMUL_HTTP_ME = os.path.join(BASE_DIR, "httpParser")
EMUL_HTTP_PROF = os.path.join(BASE_DIR, "../httpparser")
ALL_FILES = glob.glob(os.path.join(BASE_DIR, "../../Tests/Only4G33ks/testFile/test?.txt"))
ALL_FILES.sort()


class TestHTTP:
    @pytest.mark.parametrize("filename", ALL_FILES)
    def testFiles(self, filename):
        name = os.path.splitext(filename)[0]

        a = subprocess.run([EMUL_HTTP_ME, filename], capture_output=True, timeout=5).stdout
        # os.system(f"./{EMUL_HTTP_ME} {filename} > {name}.me")
        os.system(f"./{EMUL_HTTP_PROF} {filename} > {name}.out")

        open(f"{name}.me", "wb").write(a)
        # my_resultat = open(f"{name}.me", "rb")
        prof_resultat = open(f"{name}.out", "rb")

        # a = my_resultat.read().strip(b'\n').split(b'\n')
        a = a.strip(b'\n').split(b'\n')
        b = prof_resultat.read().strip(b'\n')

        # if b'IPv' in b:
        #     pytest.skip("IPv6 non implémenté par le parseur")

        b = b.split(b'\n')

        if not a:
            pytest.fail("Sortie vide", pytrace=False)

        if len(a) != len(b):
            pytest.fail("Longueurs différentes", pytrace=False)

        depth = -1
        for i in range(len(a)):
            if b'[1:header_field] = "Hos' in b[i]:  # si on est sur un header Host
                depth = self.getDepth(b[i])
            elif self.getDepth(b[i]) == depth:
                depth = -1

            if depth != -1:
                continue
            if a[i] != b[i]:
                pytest.fail(f"Error ligne {i} :\n.me  : \'{a[i]}\'\n.out : \'{b[i]}\'", pytrace=False)

    @staticmethod
    def getDepth(line: bytes) -> bytes:
        return line.strip(b' \t').replace(b'[', b'').split(b':')[0]


pytest.main(sys.argv)
