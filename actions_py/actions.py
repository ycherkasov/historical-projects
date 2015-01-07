# -*- coding: utf-8 -*-
import os
import subprocess
import stat
import errno
import shutil
import zipfile
import distutils.dir_util as distutils_dir

__doc__ = ''' Action class and derived from it classes implement interface for the Command pattern
They should support do() and undo() methods.
do() method contains logic of some action (mapping network drive, copy fines, register library)
undo() method should contain rollback action if necessary (delete copied files, unregister library, etc)

ActionList class call actions from the list which it contains and push undo() methods to the stack
In case of catching an exception undo() stack start to execute.
If error occurs during undo() action, error is reported
'''


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


class Action():
    '''
    Action (Command pattern) base class
    Use do() method to implement action, undo() to roll it back
    '''

    simulate = False

    def __init__(self, description, no_rollback=False):
        self.__description = description
        self.__no_rollback = no_rollback

    @property
    def description(self):
        return self.__description

    @property
    def no_rollback(self):
        '''
        Rollback does not performed for that Action instance
        '''
        return self.__no_rollback

    def do(self):
        '''
        Should no be called directly
        '''
        raise NotImplementedError('implement do() method in the class derived of Action')

    def undo(self):
        '''
        Should no be called directly
        '''
        raise NotImplementedError('implement do() method in the class derived of Action')


class MakeDirAction(Action):
    '''
    Standard action: Create directory
    '''

    def __init__(self, dir_name):
        Action.__init__(self, 'Create directory {0}'.format(dir_name))
        self.dir_name = dir_name

    def do(self):
        '''
        Create a directory if does not exists
        '''
        if Action.simulate:
            return
        if not os.path.exists(self.dir_name):
            os.makedirs(self.dir_name)

    def undo(self):
        '''
        Rollback: remove created directory if does not exists
        '''
        if Action.simulate or Action.no_rollback:
            return
        if os.path.exists(self.dir_name):
            shutil.rmtree(self.dir_name, onerror=on_rm_error)


class CopyFilesAction(Action):
    '''
    Standard action: Copy directory tree
    '''

    def __init__(self, source, destination):
        Action.__init__(self, 'Copying {0} to {1}'.format(source, destination))
        self.source = source
        self.destination = destination

    def do(self):
        '''
        Copy directory tree
        '''
        if Action.simulate:
            return
        distutils_dir.copy_tree(self.source, self.destination)

    def undo(self):
        '''
        Rollback: unable to remove only copied files from the directory
        '''
        pass


class CopyFileAction(Action):
    '''
    Standard action: Copy single file or directory contents (without subdirectories)
    '''

    def __init__(self, source, destination):
        '''
        Standard action: copy single file
        :param source: source file name
        :param destination: destination file name
        '''
        Action.__init__(self, 'Copying {0} to {1}'.format(source, destination))
        assert(os.path.isfile(source))
        os.path.isfile(destination)
        self.source = source
        self.destination = destination

    @staticmethod
    def silent_remove(filename):
        try:
            os.remove(filename)
        except OSError as e:
            # errno.ENOENT = no such file or directory
            # re-raise exception if a different error occured
            if e.errno != errno.ENOENT:
                raise

    def do(self):
        '''
        Copy single file or directory contents (without subdirectories)
        '''
        if Action.simulate:
            return
        CopyFileAction.silent_remove(self.destination)
        shutil.copyfile(self.source, self.destination)

    def undo(self):
        '''
        Rollback: remove single file
        '''
        if Action.simulate or Action.no_rollback:
            return
        CopyFileAction.silent_remove(self.destination)


class ExecuteAction(Action):
    '''
    Standard action: Execute command string
    '''

    def __init__(self, command):
        Action.__init__(self, 'Execute {0}'.format(command))
        self.command = command

    def do(self):
        '''
        Execute shell command
        '''
        if Action.simulate:
            return
        if not (0 == os.system(self.command)):
            raise RuntimeError('Error execute {0}'.format(self.command))

    def undo(self):
        '''
        Rollback: Do nothing
        '''
        pass


class EnvironmentVariableAction(Action):
    '''
    Standard action related to environment variables modification.
    Abstract class, as variable could be added, modified or removed
    '''
    # flag that informs about EnvironmentVariableAction single usage
    # actual modification is performed only after script finish his work
    used_once = False

    path_registry_entry = r'HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment'

    def __init__(self, description, env_variable_name):
        '''
        :param description: action description string that should be passed to the base class
        :param env_variable_name: name of the environment variable e.g. PATH, SYSTEM_DIR etc
        '''
        # pass action description
        Action.__init__(self, description)

        # variable we're going to modify
        self.__env_variable_name = env_variable_name

        # Update env workaround:
        # Calling ReplacePathAction twice could generate PATH errors,
        # as actual PATH value is replaced after script finished
        if EnvironmentVariableAction.used_once:
            single = 'Calling ReplacePathAction twice could generate PATH errors'
            raise RuntimeError(single)

    def modify_variable(self, env_value):
        '''
        Modify environment variable with value provided
        :param env_value: new value of the environment variable
        '''
        ex_string = 'reg add "{0}" /f /v {1} /t REG_SZ /d "{2}"'.format(
            EnvironmentVariableAction.path_registry_entry, self.__env_variable_name, env_value)
        #logger.info('Execute string: {0}'.format(ex_string))
        #print 'Execute string: {0}'.format(ex_string)
        EnvironmentVariableAction.used_once = True
        return os.system(ex_string)

    def delete_variable(self):
        '''
        Delete environment variable
        '''
        ex_string = 'reg delete "{0}" /v {1} /f'.format(EnvironmentVariableAction.path_registry_entry,
                                                        self.__env_variable_name)

        #logger.info('Execute string: {0}'.format(ex_string))
        return os.system(ex_string)

    @property
    def env_variable_name(self):
        '''
        :return : name of the environment variable e.g. PATH, SYSTEM_DIR etc
        '''
        return self.__env_variable_name


class ReplaceEnvironmentVariableAction(EnvironmentVariableAction):
    '''
    Standard action: Replace substrings in the existing env variable
    Do not perform that action more than one time in the application
    If so it could generate errors, as actual env value is replaced after script finished
    Calling it twice makes re-read of old env value and clean your first replacement
    '''

    def __init__(self, env_variable_name, replace_pairs):
        '''
        :param env_variable_name: environment variable name
        :param replace_pairs: list of string pairs, where 1st element is substring to replace,
        2nd is substring replace with
        :type replace_pairs: list of tuples (string, string)
        :raise: RuntimeError - in case of called twice in one application
        '''
        EnvironmentVariableAction.__init__(self,
                                           'Replace list of substrings in the env variable',
                                           env_variable_name)
        # list of string pairs to replace e.g. ("replace_from", "replace_to")
        self.replace_pairs = replace_pairs

        # backup old variable value
        self.old_value = ''

    def __replace_variable_substring(self):
        '''
        :return: 'reg add' replacing environment variable value return code
        '''
        #logger.info('Modify env {0} variable'.format(self.env_variable_name))

        self.old_value = os.environ[self.env_variable_name]

        #logger.info('Old env value: {0}'.format(self.old_value))

        # do string replacement
        env_value = self.old_value
        for old_substr, new_substr in self.replace_pairs:
            #logger.info('Replace substring in the {0}: {1} to {1}'.format(
            # self.env_variable_name,old_substr,new_substr))
            env_value = env_value.replace(old_substr, new_substr)

        # do actual env variable modification
        #logger.info('New env value: {0}'.format(env_value))
        return self.modify_variable(env_value)

    def __restore_old_path(self):
        '''
        @return: 'reg add' replacing PATH value return code
        '''
        # if called without old value backed up, do nothing
        if not getattr(self, 'old_value'):
            return
        return self.modify_variable(self.old_value)

    def do(self):
        '''
        Do replacement in the PATH env value
        :raise: RuntimeError - if reg add returned a non-zero error code
        '''
        if Action.simulate:
            return
        if 0 != self.__replace_variable_substring():
            raise RuntimeError('Error while changing the env value')

    def undo(self):
        '''
        Undo changing env value
        '''
        if Action.simulate or Action.no_rollback:
            return
        if 0 != self.__restore_old_path():
            print('Unable to change env back')
            #logger.error('Unable to change PATH back')


class AppendEnvironmentVariableAction(EnvironmentVariableAction):
    '''
    Action: Append substring to the existing environment variable
    '''
    def __init__(self, env_variable_name, new_value):
        '''
        :param env_variable_name: environment variable name
        :param new_value: new value of 'env_variable_name'
        :type new_value: string
        :raise: RuntimeError - in case of called twice in one application
        '''
        EnvironmentVariableAction.__init__(self,
                                           'Append new value to the environment variable',
                                           env_variable_name)
        self.new_value = new_value

        assert isinstance(self.env_variable_name, basestring)
        self.old_value = os.environ[self.env_variable_name]

    def __add_path_entries(self):
        '''
        @return: 'reg add' editing PATH value return code
        '''
        #logger.info('Append to env variable')
        #logger.info('Old env value: {0}'.format(self.old_value))

        # do actual append
        env_value = self.old_value
        #print 'Old env value [{0}]'.format(env_value)
        #print 'New env value [{0}]'.format(self.new_value)

        # append if does not exist only
        if -1 == env_value.find(self.new_value):
            env_value += ';'
            env_value += self.new_value

        # replace double ;; is accidentally appear
        env_value.replace(';;', ';')

        # do actual env variable modification
        #print('New env value: {0}'.format(env_value))
        return self.modify_variable(env_value)

    def __restore_old_path(self):
        '''
        @return: 'reg add' replacing PATH value return code
        '''
        # if called without old value backed up, do nothing
        if not getattr(self, 'old_value'):
            return
        return self.modify_variable(self.old_value)

    def do(self):
        '''
        Append to the PATH env value
        :raise: RuntimeError - if reg add returned a non-zero error code
        '''
        if Action.simulate:
            return
        if 0 != self.__add_path_entries():
            raise RuntimeError('Error while changing the evn value')

    def undo(self):
        '''
        Undo changing PATH env
        '''
        if Action.simulate:
            return
        if 0 != self.__restore_old_path():
            print('Unable to change back env value')
            #logger.error('Unable to change back env value')


class AddEnvironmentVariableAction(EnvironmentVariableAction):
    '''
    Action: create new environment variable
    '''
    def __init__(self, new_env_variable_name, new_value):
        '''
        :param new_env_variable_name: environment variable name
        :param new_value: new value of 'env_variable_name'
        :type new_value: string
        :raise: RuntimeError - in case of called twice in one application
        '''
        assert isinstance(new_env_variable_name, basestring)
        EnvironmentVariableAction.__init__(self,
                                           'Append new value to the environment variable',
                                           new_env_variable_name)
        self.new_value = new_value

    def __add_new_env_variable(self):
        '''
        @return: 'reg add' editing PATH value return code
        '''
        # do actual env variable modification
        #logger.info('New env value: {0}'.format(self.new_value))
        return self.modify_variable(self.new_value)

    def __restore_old_path(self):
        '''
        @return: 'reg add' replacing PATH value return code
        '''
        # if called without old value backed up, do nothing
        # if called without old value backed up, do nothing
        return self.delete_variable()

    def do(self):
        '''
        Append to the PATH env value
        :raise: RuntimeError - if reg add returned a non-zero error code
        '''
        if Action.simulate:
            return
        if 0 != self.__add_new_env_variable():
            raise RuntimeError('Error while changing the evn value')

    def undo(self):
        '''
        Do not rollback
        '''
        pass


class UnpackAction(Action):
    '''
    Standard action: Unpack files from the zip archive
    '''

    def __init__(self, archive, destination):
        '''
        :param archive: Full archive file name
        :param destination: Directory where to unpack
        '''
        Action.__init__(self, 'Unpack files from zip archive {0} to {1}'.format(archive, destination))
        self.archive = archive
        self.destination = destination
        self.unpacked = ''

    def do(self):
        '''
        Unpack zip archive to the destination
        '''
        if Action.simulate:
            return
        unpacked_dir_name, _ = os.path.splitext(self.archive)
        self.unpacked = os.path.join(self.destination, unpacked_dir_name)
        if os.path.exists(self.unpacked):
            shutil.rmtree(self.unpacked, onerror=on_rm_error)
        with zipfile.ZipFile(self.archive, 'a') as build:
            build.extractall(self.destination)

    def undo(self):
        '''
        Rollback: Delete unpacked files
        '''
        if Action.simulate or Action.no_rollback:
            return
        shutil.rmtree(self.unpacked, onerror=on_rm_error)


class RenameAction(Action):
    '''
    Standard action: Rename file or directory
    '''

    def __init__(self, old_name, new_name):
        '''
        :param old_name: old file name
        :param new_name: new file name
        '''
        Action.__init__(self, 'Rename from {0} to {1}'.format(old_name, new_name))
        self.old_name = old_name
        self.new_name = new_name

    def do(self):
        '''
        Rename file or directory
        '''
        if Action.simulate:
            return
        if os.path.exists(self.new_name):
            shutil.rmtree(self.new_name, onerror=on_rm_error)
        os.rename(self.old_name, self.new_name)

    def undo(self):
        '''
        Rename back file or directory
        '''
        if Action.simulate or Action.no_rollback:
            return
        os.rename(self.new_name, self.old_name)


class MapDriveAction(Action):
    '''
    Action (Command pattern) that do connection to the remote drive
    '''

    def __init__(self, drive_letter, drive_path):
        '''
        :param drive_letter: Drive letter where to map network path (e.g. Z:)
        :param drive_path: network path in UNC format (e.g. \\server\share)
        '''
        Action.__init__(self, 'Map drive {0}: to the network path {1}'.format(drive_letter, drive_path))
        self.drive_letter = drive_letter
        self.drive_path = drive_path
        # do not unmap drive in case of error (should be connected constantly)
        self.unmap = False

    def __connect(self):
        '''
        Implement connection of the network drive
        '''
        if os.path.exists('{0}:'.format(self.drive_letter)):
            return 0
        retcode = subprocess.call(r'net use {0}: {1}'.format(self.drive_letter, self.drive_path), shell=True)
        return retcode

    def __disconnect(self):
        '''
        Implement disconnection of the network drive
        '''
        if not os.path.exists('{0}:'.format(self.drive_letter)):
            return 0
        retcode = subprocess.call(r'net use {0}: /del'.format(self.drive_letter), shell=True)
        return retcode

    def do(self):
        '''
        Tries to map a network drive. Raise a runtime exception if shell command returns non-zero.
        '''
        retcode = 0
        # do not unmap by default
        if (not Action.simulate) and self.unmap:
            retcode = self.__connect()
        if not (0 == retcode):
            raise RuntimeError('Error drive mapping action')

    def undo(self):
        '''
        Do not rollback, we need persistent connection
        '''
        pass


class ActionList():
    '''
    '''

    def __init__(self, actions_list):
        '''
        :param actions_list: list of object instances derived from Action class (support do() and undo() interface)
        '''
        map(lambda action: self.__check_action_interface(action), actions_list)
        self.actions_list = actions_list
        self.undo_list = []
        self.log_handler = None

    def add_log_handler(self, log_handler):
        '''
        :param log_handler: object that support standard Python logger interface (info(), error(), debug() and so on)
        '''
        self.log_handler = log_handler

    def add_action(self, action):
        '''
        Add a single action to the list
        :type action: Action
        :param action: object instance derived from Action class (support do() and undo() interface)
        '''
        self.__check_action_interface(action)
        self.actions_list.append(action)

    def __check_action_interface(self, action):
        '''
        Perform interface check (should derive from the Action class)
        :type action: Action
        :param action: object instance derived from Action class (support do() and undo() interface)
        '''
        if not (callable(getattr(action, 'do')) and callable(getattr(action, 'undo'))):
            raise NotImplementedError('In the action you trying to add does not provided do() or undo() methods')

    def process(self):
        '''
        Execute all actions in the list. Place successful undo() actions to the stack
        '''
        try:
            for action in self.actions_list:
                if self.log_handler is not None:
                    self.log_handler.info(action.description)
                self.undo_list.append(action)
                action.do()

        except:
            for undo_action in reversed(self.undo_list):
                self.log_handler.info('Rollback [{0}]'.format(undo_action.description))
                undo_action.undo()
            raise
