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
import os
import stat
from zipfile import ZipFile

class CreateProjectExtractForge(Plugin):
    def __init__(self):
        Plugin.__init__(self, "mcmod.createproject.setup", 20)
        self.name = "CreateProjectExtractForge"
        self.title = "Extract Minecraft Forge"
        self.description = "Plugin to handle extracting forge to the desired destination."
        self.author = "edmiester777"
        self.version = "0.0.1"
        self.status_text = "Unzipping Minecraft Forge..."

    def exec_hook(self, directory, runtimeconfig):
        forge_file = os.path.join(directory, runtimeconfig["DownloadsDirectory"], "forge.zip")
        target_dir = os.path.join(directory, runtimeconfig["ForgeDirectory"])
        with ZipFile(forge_file, "r") as zip_ref:
            zip_ref.extractall(target_dir)

        if not os.name == 'nt':
            self.logger.debug("Attempting to make gradlew executable.")
            gradlew = os.path.join(directory, runtimeconfig["ForgeDirectory"], "gradlew")
            st = os.stat(gradlew)
            os.chmod(gradlew, st.st_mode | stat.S_IEXEC)
        return True

# Registering this plugin...
register_plugin(CreateProjectExtractForge())