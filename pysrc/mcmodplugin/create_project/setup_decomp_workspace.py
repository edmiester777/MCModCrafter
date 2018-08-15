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
from mcmodplugin.helper import gradle_helper
import os
from zipfile import ZipFile

class CreateProjectSetupDecompWorkspace(Plugin):
    def __init__(self):
        Plugin.__init__(self, "mcmod.createproject.setup", 30)
        self.name = "CreateProjectSetupDecompWorkspace"
        self.title = "Setup Decompiled Workspace"
        self.description = "Plugin to handle setting up a decompiled minecraft forge workspace."
        self.author = "edmiester777"
        self.version = "0.0.1"
        self.status_text = "Working on it..."

    def exec_hook(self, directory, runtimeconfig):
        gradle_helper.runTarget(directory, runtimeconfig, "setupDevWorkspace", self.update_status)
        return True
        
    def update_status(self, status):
        self.logger.debug('>>> %s' % (status))
        self.status_text = status

# Registering this plugin...
register_plugin(CreateProjectSetupDecompWorkspace())