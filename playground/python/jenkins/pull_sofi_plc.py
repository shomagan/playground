import os
from git import Repo
from git import Git
import last_version


def main():
    git_ssh_identity_file = os.path.expanduser(last_version.id_rsa_path)
    git_ssh_cmd = 'ssh -i %s' % git_ssh_identity_file
    with Git().custom_environment(GIT_SSH_COMMAND=git_ssh_cmd):
        repo = Repo(last_version.sofi_plc_project_path)
        assert not repo.bare
        repo.git.reset('--hard')
        repo.git.clean('-df')
        repo.remotes.origin.pull()
        result = "pulled"
    print(result)
    return result


if __name__ == "__main__":
    main()

