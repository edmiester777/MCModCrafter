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

import os
import subprocess

##
# @brief Run a target on gradle project.
#
# This static function helps you spawn the subprocess and read output
# in a standard way for a target gradle command.
#
# @param projectdir Directory for project.
# @param runtimeconfig RuntimeConfig passed to the calling plugin.
# @param target Target command to execute on gradle.
# @param on_output Callable method which we should call when we receive output.
def runTarget(projectdir, runtimeconfig, target, on_output):
    forge_dir = os.path.join(projectdir, runtimeconfig["ForgeDirectory"])
    executable = os.path.join(forge_dir, "gradlew")
    p = subprocess.Popen([executable, target], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=forge_dir)
    for line in iter(p.stdout.readline, b''):
        if on_output is not None:
            on_output(line.rstrip())