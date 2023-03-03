import glob
import os
import subprocess
import sys

import pytest

BASE_DIR = os.environ.get("BASE_DIR", ".")
EMUL_HTTP = os.path.join(BASE_DIR, "structure")
ALL_FILES = glob.glob(os.path.join(BASE_DIR, "../Tests/testgrammar/testFile/*.txt"))
ALL_FILES.sort()


class TestHTTP:
    @pytest.mark.parametrize("filename", ALL_FILES)
    def testFiles(self, filename):
        output = os.system(f"./{EMUL_HTTP} {filename}")

        if output != 0:
            pytest.fail("Error")


pytest.main(sys.argv)
