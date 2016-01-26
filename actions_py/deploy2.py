import os
import sys
import logging
import argparse
import uuid
from tc_agent_action import Action, ActionList, CopyFileAction, CopyFilesAction
from tc_agent_action import MapDriveAction, MakeDirAction, UnpackAction
from tc_agent_action import AddEnvironmentVariableAction, AppendEnvironmentVariableAction


__doc__ = '''The script automates TC build agent deployment process

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
###################################################


class InstallVisualStudioAction(Action):
    '''
    Install Visual Studio 2012 and Service Pack 3
    '''

    vs_install_command = '{0}\\vs_professional.exe /adminfile {0}\\AdminDeployment.xml /quiet /norestart'
    sp_install_command = '{0}\\SP3\VS2012.3.exe /quiet'

    def __init__(self, network_share):
        '''
        :param network_share: UNC path to the share where unattended package of Visual Studio 2012 is placed
        '''
        Action.__init__(self, 'Install Visual Studio 2012 and Service Pack 3 from {0}'.format(network_share))
        self.network_share = network_share

    @staticmethod
    def __install_visual_studio(path):
        '''
        :param path: path to the network drive where VS package is placed
        '''
        return os.system(InstallVisualStudioAction.vs_install_command.format(path))

    @staticmethod
    def __install_service_pack(path):
        '''
        :param path: path to the network drive where VS package is placed
        '''
        return os.system(InstallVisualStudioAction.sp_install_command.format(path))

    def do(self):
        '''
        Install Visual Studio 2012 and Service Pack 3
        '''
        if Action.simulate:
            return
        if 0 != InstallVisualStudioAction.__install_visual_studio(self.network_share):
            raise RuntimeError('Error while installing Visual Studio')
        if 0 != InstallVisualStudioAction.__install_service_pack(self.network_share):
            raise RuntimeError('Error while installing Visual Studio Service Pack 3')

    def undo(self):
        '''
        We can't do nothing in case of installation error.
        The problem should be investigated manually
        '''
        pass


class InstallGitAction(Action):
    '''
    Install Git client for Windows
    '''
    git_install_command = r'''{0}\git-install.exe /verysilent /COMPONENTS="icons,ext\reg\shellhere,assoc,assoc_sh"'''

    git_exe_path = r'"C:\Program Files (x86)\Git\bin\git.exe"'
    git_config_path = r'"C:\Program Files (x86)\Git\etc\gitconfig"'
    skip_ssl_command = '"{0} config --file {1} --add http.sslverify false"'.format(git_exe_path, git_config_path)

    keytool_connamd = r'"C:\Program Files (x86)\Java\jre6\bin\keytool.exe"'
    path_to_certificate = r'"X:\git\certificate\db-root-ca-4.pem"'
    path_to_keystore = r'"C:\Program Files (x86)\Java\jre6\lib\security\cacerts"'
    install_certificate = '"{0} -importcert -file {1} -alias {2} -keystore {3} -storepass changeit -noprompt"'.format(
        keytool_connamd,
        path_to_certificate,
        uuid.uuid1(),
        path_to_keystore)

    def __init__(self, network_share):
        '''
        :param network_share: UNC path to the share where unattended package of Visual Studio 2012 is placed
        '''
        Action.__init__(self, 'Install Git client for Windows from {0}'.format(network_share))
        self.network_share = network_share

    @staticmethod
    def __install_git(path):
        '''
        Install Git client command
        :param path: path to the network drive where Git package is placed
        '''
        return os.system(InstallGitAction.git_install_command.format(path))

    @staticmethod
    def __skip_ssl():
        '''
        Skip SSL verification command
        '''
        return os.system(InstallGitAction.skip_ssl_command)

    @staticmethod
    def __install_certificate():
        '''
        Install SSL certificate command
        '''
        return os.system(InstallGitAction.install_certificate)

    def do(self):
        '''
        Install Git client for Windows
        '''
        if Action.simulate:
            return
        if 0 != InstallGitAction.__install_git(self.network_share):
            raise RuntimeError('Error while installing Git')
        if 0 != InstallGitAction.__skip_ssl():
            raise RuntimeError('Error while configuring Git')
        if 0 != InstallGitAction.__install_certificate():
            raise RuntimeError('Error while installing SSL certificate')

    def undo(self):
        '''
        Do not rollback
        '''
        pass


class InstallJavaAction(Action):
    '''
    Install Java 6 and JDK
    '''

    java_install_command = r'X:\Java\jdk-6u38-windows-i586.exe /s'

    def __init__(self):
        Action.__init__(self, 'Install Java 6 and JDK')

    def do(self):
        '''
        Install Java 6 and JDK
        '''
        if Action.simulate:
            return
        if 0 != os.system(InstallJavaAction.java_install_command):
            raise RuntimeError('Error while installing JDK')

    def undo(self):
        '''
        We can't do nothing in case of installation error.
        The problem should be investigated manually
        '''
        pass


def get_standard_actions():
    '''
    Call that function to get a list of standard agent deployment actions "from the scratch"
    :rtype : list of Actions
    :return : Deployment actions sequence
    '''
    zip_archive = r'C:\build.zip'
    return [

        # map drives
        MapDriveAction('X', r'\\lonfxrmsvsu59\distrib'),
        MapDriveAction('W', r'\\lonfxrmsvsu59\TeamCityShare'),

        # copy files
        CopyFileAction(r'W:\build\build.zip', zip_archive),

        # unpack files
        UnpackAction(zip_archive, 'C:\\'),

        # Install SysInternals
        CopyFilesAction(r'X:\SysInternals', r'C:\Windows'),

        # Create directory for the riskstore
        MakeDirAction(r'D:\DATA\RMS_Riskstore'),

        # install visual studio and SP3
        InstallVisualStudioAction(r'W:\IDEInstall'),

        # install JDK
        InstallJavaAction(),

        # update environment variables
        AddEnvironmentVariableAction('RSA_BUILD_ENV_ROOT', r'C:\build'),
        AddEnvironmentVariableAction('JAVA_HOME', r'C:\Program Files (x86)\Java\jdk1.6.0_38')]


def get_build_deploy_actions():
    '''
    Get the actions list that redeploys build.zip
    :rtype : list of Actions
    :return : Deployment actions sequence
    '''
    zip_archive = r'C:\build.zip'
    return [
        CopyFileAction(r'W:\build\build.zip', zip_archive),

        # unpack files
        UnpackAction(zip_archive, 'C:\\')]


def get_custom_actions():
    '''
    Call that function to get a custom list of deployment actions
    :rtype : list of Actions
    :return : Deployment actions sequence
    '''
    # ADD YOUR CUSTOM ACTIONS HERE!
    logger.warning('Start custom deployment process. Check get_custom_actions() function contents!')

    return [
        InstallGitAction(r'X:\git'),

        AppendEnvironmentVariableAction('PATH', r'C:\Program Files (x86)\Git\bin')
    ]


###################################################
def main():
    parser = argparse.ArgumentParser(description='Command-line interface')
    parser.add_argument('--simulate',
                        help='Do not actually perform actions, just simulate (debug and logs inspection)',
                        action='store_true',
                        default=False,
                        required=False)

    parser.add_argument('--complete',
                        help='Perform complete deployment, call custom deployment otherwise (requires code edit)',
                        action='store_true',
                        default=False,
                        required=False)

    parser.add_argument('--redeploy',
                        help='Perform redeploy of the build directory and *.ini files copying',
                        action='store_true',
                        default=False,
                        required=False)

    parser.add_argument('--custom',
                        help='Perform series of custom actions defined in the get_custom_actions() function',
                        action='store_true',
                        default=False,
                        required=False)

    args = parser.parse_args()
    Action.simulate = args.simulate

    try:
        assert isinstance(args.complete, bool)
        assert isinstance(args.redeploy, bool)
        if args.redeploy:
            actions = get_build_deploy_actions()
        elif args.complete:
            actions = get_standard_actions()
        elif args.custom:
            actions = get_custom_actions()
        else:
            parser.print_help()
            sys.exit(0)

        logger.info('Start deployment process...')
        action_list = ActionList(actions)
        action_list.add_log_handler(logger)

        # perform actual deployment
        action_list.process()

        return 0

    except RuntimeError as runtime_error:
        print 'Runtime error: {0}'.format(runtime_error)
        return 1
    except IOError as ioerror:
        print 'I/O error({0}): {1}'.format(ioerror.errno, ioerror.strerror)
        return ioerror.errno


if __name__ == '__main__':
    sys.exit(main())
