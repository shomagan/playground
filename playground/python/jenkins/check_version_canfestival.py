import os
import last_version
from git import Repo
from git import Git
__description__ = 'check version for canfestival module'
from telegram_api import send_message


def main():
    git_ssh_identity_file = os.path.expanduser(last_version.id_rsa_path)
    git_ssh_cmd = 'ssh -i %s' % git_ssh_identity_file
    result = "the_same"
    with Git().custom_environment(GIT_SSH_COMMAND=git_ssh_cmd):
        repo = Repo(last_version.sofi_plc_project_path + '/free_rtos/canfestival')
        assert not repo.bare
        last_tag = repo.git.tag('--points-at', 'HEAD')
        tags = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        last_version.delete_unpaired_tags_from_local(repo, tags, last_tag)
        tags_local = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        origin = repo.remotes.origin
        origin.fetch()
        tags_remote = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        last_version.delete_remote_tags_from_local(repo, tags_local, tags_remote)
        if tags_local[-1] != tags_remote[-1]:
            result = "changed"
            send_message("canfestival version changed")
        else:
            send_message("canfestival have the same version")
    print(result)
    return result


if __name__ == "__main__":
    main()
