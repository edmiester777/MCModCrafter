#########################################################################
# MCModCrafter Program to help introduce people to minecraft modding and
# to speed development time via logic visualization.
#
# Copyright (C) 2017 Edward Callahan
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#########################################################################

from mcmod import Plugin, register_plugin
from zipfile import ZipFile
from javalang.parse import parse
import javalang
import json
import os

class Serializable:
    def toDict(self):
        raise RuntimeError("Did not implement the toDict method.")

    def __str__(self):
        return json.dumps(self.toDict())

class JavaFileRepresentation(Serializable):
    def __init__(self, ast):
        self.package = None
        self.class_name = None
        self.class_modifier = None
        self.__parse_from_ast(ast)

    def __parse_from_ast(self, ast):
        if ast.package is not None:
            self.package = ast.package.name

    def toDict(self):
        return {
            "package": self.package,
            "class_name": self.class_name,
            "class_modifier": self.class_modifier
        }


class ParseJavaSource(Plugin):
    def __init__(self):
        Plugin.__init__(self, "mcmod.init", 500)
        self.name = "ParseJavaSource"
        self.title = "Parse Java Source Code"
        self.description = "This plugin takes care of parsing the java source " \
            "code into a readable format by python."
        self.author = "edmiester777"
        self.version = "0.0.1"
        self.status_text = "Parsing..."

        self._runtimeconfig = None
        self._output_path = None
        self._java_home = None
        self._java_src_path = None
        self._java_src_extract_path = None
        self._java_namelist = None

    def exec_hook(self, runtimeconfig):
        self._runtimeconfig = runtimeconfig
        self._check_paths()
        self._verify_python_home()
        self._extract_src()
        self._parse_src()

    ##
    # Make sure the required paths exist.
    def _check_paths(self):
        self.status_text = "Checking paths..."
        path = os.path.join(os.path.abspath("."), self._runtimeconfig["ParseDirectory"])
        if not os.path.exists(path):
            os.makedirs(path)
        self._output_path = path
        self._java_src_extract_path = os.path.join(path, "extract")

    ##
    # Validate that JAVA_HOME has been set.
    def _verify_python_home(self):
        self.status_text = "Verifying environment..."
        if not os.environ.has_key("JAVA_HOME"):
            self.logger.fatal("Cannot find JAVA_HOME.")
            raise RuntimeError("JAVA_HOME has not been set.")
        self._java_home = os.environ["JAVA_HOME"]

        java_src = os.path.join(self._java_home, "src.zip")
        if not os.path.exists(java_src):
            self.logger.fatal("Java src.zip does not exist.")
            raise RuntimeError("JAVA_HOME does not contain a src.zip... Are you sure JAVA_HOME is jdk?")
        self._java_src_path = java_src

    ##
    # Extract the java source into the desired directory.
    def _extract_src(self):
        with ZipFile(self._java_src_path, "r") as zf_ref:
            # getting namelist
            namelist = [name for name in zf_ref.namelist() if name.endswith(".java")]
            self._java_namelist = namelist
            for name in namelist:
                file_path = os.path.join(self._java_src_extract_path, name)
                if not os.path.exists(file_path):
                    self.status_text = "Extracting %s..." % (name)
                    self.logger.debug("Extracting %s..." % (name))
                    zf_ref.extract(name, path=self._java_src_extract_path)

    ##
    # Parse the files in the namelist.
    def _parse_src(self):
        i = 0
        for name in self._java_namelist:
            i += 1
            file = os.path.join(self._java_src_extract_path, name)
            msg = "Parsing %s (%s/%s)..." % (name, i, len(self._java_namelist))
            self.status_text = msg
            self.logger.debug(msg)
            with open(file, 'r') as javafile:
                src = javafile.read()
                ast = parse(src)
                clsdescriptor = JavaFileRepresentation(ast)
                print clsdescriptor
        

# Registering this plugin...
register_plugin(ParseJavaSource())