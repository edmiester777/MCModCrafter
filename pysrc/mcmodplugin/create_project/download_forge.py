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

class CreateProjectDownloadForge(Plugin):
    def __init__(self):
        Plugin.__init__(self, "mcmod.createproject.setup", 10)
        self.name = "CreateProjectDownloadForge"
        self.description = "Plugin to handle the first stage of creating a project. "\
            "This will download minecraft forge to the project directory."
        self.author = "edmiester777"
        self.version = "0.0.1"
        self.status_text = "Downloading Minecraft Forge..."

    def exec_hook(self, directory, runtimeconfig):
        return True

# Registering this plugin...
register_plugin(CreateProjectDownloadForge())