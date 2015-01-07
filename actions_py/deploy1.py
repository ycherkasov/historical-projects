# -*- coding: utf-8 -*-
import sys
import os
import shutil
import logging
import argparse
import stat
import errno
import re
from subprocess import call
from tc_agent_action import Action, RenameAction, CopyFilesAction, ActionList, MapDriveAction
from tc_agent_action import ReplaceEnvironmentVariableAction, AppendEnvironmentVariableAction


__doc__ = '''Deploy DBAnalytics distribution to the TC build box script
Support upgrade of existing installation and deployment from scratch
'''


###################################################
# Script configuration
###################################################

# Set to True if you want to redirect output to a logfile as well as console
logToFile = False

###################################################
# create and configure logger
# create console handler and logfile handlers
logger = logging.getLogger('deploy_tc_agent')
logger.setLevel(logging.DEBUG)

# change handler.setLevel() to DEBUG for verbose output
# change handler.setLevel() to INFO for standard output
console = logging.StreamHandler()
console.setLevel(logging.INFO)
formatter = logging.Formatter('[%(asctime)s - %(levelname)s] %(message)s')
console.setFormatter(formatter)
logger.addHandler(console)

if logToFile:
    logfile = logging.FileHandler('deploy_tc_agent.log', mode='w')
    logfile.setLevel(logging.DEBUG)
    logfile.setFormatter(formatter)
    logger.addHandler(logfile)


###########################################################################
def on_rm_error(*args):
    '''
    In case the file or directory is read-only and we need to delete it
    this function will help to remove 'read-only' attribute
    :param args: (func, path, exc_info) yuple
    '''
    # path contains the path of the file that couldn't be removed
    # let's just assume that it's read-only and unlink it.
    _, path, _ = args
    os.chmod(path, stat.S_IWRITE)
    os.unlink(path)


def copy_anything(src, dst):
    '''

    :param src:
    :param dst:
    :raise: everything except OSError not a directory
    '''
    try:
        shutil.copytree(src, dst)
    except OSError as exc:  # python >2.5
        if exc.errno == errno.ENOTDIR:
            shutil.copy(src, dst)
        else:
            raise


###########################################################################
# unregister old
class UnregisterAction(Action):
    '''
    Standard action: Unregister the library
    '''

    def __init__(self, library_path):
        '''
        :param library_path: full path to the DLL or included to PATH env
        '''
        Action.__init__(self, 'Unregister library {0}'.format(library_path))
        self.library_path = library_path

    def do(self):
        '''
        Try to replace DBAX path string in the PATH env variable
        :raise: RuntimeError - if regsvr32 returned a non-zero error code
        '''
        retcode = 0
        if not Action.simulate:
            retcode = call(["regsvr32", "/s", "/u", self.library_path])
        if not (0 == retcode):
            raise RuntimeError('Error while unregistering {0}'.format(self.library_path))

    def undo(self):
        '''
        Roll back replacing DBAX path string in the PATH env variable 
        '''
        if Action.simulate:
            return
        if 0 != call(["regsvr32", "/s", self.library_path]):
            logger.error('Unable to register back {0}'.format(self.library_path))


###########################################################################
# register new
class RegisterAction(Action):
    '''
    Standard action: Register the library
    '''

    def __init__(self, library_path):
        '''
        :param library_path: full path to the DLL or included to PATH env
        '''
        Action.__init__(self, 'Register library {0}'.format(library_path))
        self.library_path = library_path

    def do(self):
        '''
        Try to register new library
        :raise: RuntimeError - if regsvr32 returned a non-zero error code
        '''
        logger.info('Register {0}'.format(self.library_path))
        retcode = 0
        if not Action.simulate:
            retcode = call(["regsvr32", "/s", self.library_path])
        if not (0 == retcode):
            raise RuntimeError('Error while registering {0}'.format(self.library_path))

    def undo(self):
        '''
        Unregister back new library
        '''
        retcode = 0
        if not Action.simulate:
            retcode = call(["regsvr32", "/s", "/u", self.library_path])
        if not (0 == retcode):
            logger.error('Unable to unregister back {0}'.format(self.library_path))


###########################################################################
def parse_dba_name(dba_name):
    '''
    :param dba_name: DBA version name (same as DBA deployment directory name)
    :return: pair (short_version, full_version). Example: ('190', '20131014.190')
    '''
    # check it match the following format: v190_20131014.190
    p = re.compile('v[0-9]{2,4}_[0-9]{8}\.[0-9]{2,4}')
    if p.match(dba_name):
        # parse to short and full versions
        _, short, full = re.split('_|v', dba_name)
        return short, full
    raise RuntimeError('DBA version should match the following format: v190_20131014.190')


###########################################################################
def update_dbanalytics(dba_installation_path, dba_distrib_path, old_dba_name, new_dba_name):
    '''
    Update DBAnalytics
    :param dba_installation_path: local path where to install DBAnalytics
    :param dba_distrib_path: network path where from install DBAnalytics
    :param old_dba_name: old DBA version in the format v190_20131014.190
    :param new_dba_name: new DBA version in the format v190_20131014.190
    :return: List of actions (objects inherited from Action)
    '''

    # old version
    old_version_num, old_version_full = parse_dba_name(old_dba_name)

    # new version
    new_version_num, new_version_full = parse_dba_name(new_dba_name)

    old_version_short = "Ver{0}".format(old_version_num)
    new_version_short = "Ver{0}".format(new_version_num)

    old_vbax_path = "{0}\\Launcher\\DBAX\\{1}\\vbax.dll".format(dba_installation_path, old_version_short)
    new_vbax_path = "{0}\\Launcher\\DBAX\\{1}\\vbax.dll".format(dba_installation_path, new_version_short)

    backup_dba_path = "C:\\DBAnalytics.{0}".format(old_version_num)

    logger.info('Old version is provided, update existing installation')
    logger.info('Simulation={0}'.format(Action.simulate))
    logger.info('Old DBAnalytics name: {0}'.format(old_dba_name))
    logger.info('New DBAnalytics name: {0}'.format(new_dba_name))
    logger.info('Old version short number: {0}'.format(old_version_short))
    logger.info('New version short number: {0}'.format(new_version_short))
    logger.info('Old version full number: {0}'.format(old_version_full))
    logger.info('New version full number: {0}'.format(new_version_full))
    logger.info('Old VBAX path: {0}'.format(old_vbax_path))
    logger.info('New VBAX path: {0}'.format(new_vbax_path))
    logger.info('DBAnalytics installed from: {0}'.format(dba_distrib_path))
    logger.info('DBAnalytics installation to: {0}'.format(dba_installation_path))
    logger.info('Old DBAnalytics backup path: {0}'.format(backup_dba_path))

    actions = [
        # map drive
        MapDriveAction('W', '\\\\lonfxrmsvsu59\\TeamCityShare'),

        # unregister old
        UnregisterAction(old_vbax_path),

        # rename old
        RenameAction(dba_installation_path, backup_dba_path),

        # copy new
        CopyFilesAction(new_dba_name, dba_installation_path),

        # register new
        RegisterAction(new_vbax_path)]

    # replace PATH
    # substring pairs to be replaced (short versions, full versions)
    replace_pairs = [(old_version_short, new_version_short), (old_version_full, new_version_full)]
    actions.append(ReplaceEnvironmentVariableAction("PATH", replace_pairs))

    return actions


###########################################################################
def install_dbanalytics(dba_installation_path, dba_distrib_path, new_dba_name):
    '''
    :param dba_installation_path: local path where to install DBAnalytics
    :param dba_distrib_path: network path where from install DBAnalytics
    :param new_dba_name: new DBA version in the format v190_20131014.190
    :return: List of actions (objects inherited from Action)
    '''

    # new version
    new_version_num, new_version_full = parse_dba_name(new_dba_name)

    new_version_short = "Ver{0}".format(new_version_num)

    new_vbax_path = "{0}\\Launcher\\DBAX\\{1}\\vbax.dll".format(dba_installation_path, new_version_short)

    logger.info('Old version is not provided, install from scratch')
    logger.info('Simulation={0}'.format(Action.simulate))
    logger.info('New version short number: {0}'.format(new_version_short))
    logger.info('New VBAX path: {0}'.format(new_vbax_path))
    logger.info('DBAnalytics installed from: {0}'.format(dba_distrib_path))
    logger.info('DBAnalytics installation to: {0}'.format(dba_installation_path))
    logger.info('New DBAnalytics version and name: {0}'.format(new_dba_name))

    actions = [
        # map drive
        MapDriveAction('W', '\\\\lonfxrmsvsu59\\TeamCityShare'),

        # copy new
        CopyFilesAction(dba_distrib_path, dba_installation_path), RegisterAction(new_vbax_path),

        # register new
        RegisterAction(new_vbax_path)]

    # append to PATH
    # Following directories should be added to PATH
    # C:\DBAnalytics
    # C:\DBAnalytics\Launcher\DBAX\Ver200
    # C:\DBAnalytics\Launcher\Addin\20140205.200
    new_paths = ';'.join(['C:\DBAnalytics',
                          'C:\\DBAnalytics\\Launcher\\DBAX\\{0}'.format(new_version_short),
                          'C:\\DBAnalytics\\Launcher\\Addin\\{0}'.format(new_version_full)])
    actions.append(AppendEnvironmentVariableAction('PATH', new_paths))

    return actions


###########################################################################
def main():
    # process command line
    parser = argparse.ArgumentParser(description='Command-line interface')
    parser.add_argument('--simulate',
                        help='Do not actually perform actions, just simulate (debug and logs inspection)',
                        action='store_true',
                        default=False,
                        required=False)
    parser.add_argument('--dba_path',
                        help='Path to the place where DBAnalytics typically deployed',
                        default='C:\\DBAnalytics',
                        required=False)
    parser.add_argument('--dba_package_path', help='Path to the network resource where DBAnalytics package is placed',
                        default='W:\\DBA',
                        required=False)
    parser.add_argument('--new_dba_name',
                        help='New DBA version name in the following format: v200_20140205.200',
                        required=True)
    parser.add_argument('--old_dba_name',
                        help='Old DBA version name in the following format: v190_20131014.190',
                        required=False)

    args = parser.parse_args()
    Action.simulate = args.simulate

    # path where to install
    dba_install_path = args.dba_path

    # path where from install
    dba_distrib_path = os.path.join(args.dba_package_path, args.new_dba_name)

    # old version
    old_dba_name = args.old_dba_name

    # new version
    new_dba_name = args.new_dba_name

    try:

        if old_dba_name is None:
            actions = install_dbanalytics(dba_install_path, dba_distrib_path, new_dba_name)
        else:
            actions = update_dbanalytics(dba_install_path, dba_distrib_path, old_dba_name, new_dba_name)

        logger.info('Start installation process...')
        action_list = ActionList(actions)
        action_list.add_log_handler(logger)
        action_list.process()
        return 0

    except RuntimeError as runtime_error:
        print 'Runtime error: {0}'.format(runtime_error)
        return 1
    except IOError as ioerror:
        print 'I/O error({0}): {1}'.format(ioerror.errno, ioerror.strerror)
        return ioerror.errno


###########################################################################
if __name__ == '__main__':
    sys.exit(main())