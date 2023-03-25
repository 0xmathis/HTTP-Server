import glob
import os
import sys

import pytest

BASE_DIR = os.environ.get("BASE_DIR", ".")
EMUL_HTTP = os.path.join(BASE_DIR, "httpParser")
ALL_FILES = glob.glob(os.path.join(BASE_DIR, "../../Tests/premier-jeu-test/get*[!'.out']"))
ALL_FILES.sort()
print(ALL_FILES)


class TestHTTP:
    @pytest.mark.parametrize("filename", ALL_FILES)
    def testFiles(self, filename):
        output = os.system(f"./{EMUL_HTTP} {filename}")

        if output != 0:
            pytest.fail("Error")


pytest.main(sys.argv)
