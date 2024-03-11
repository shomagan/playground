import os
import last_version
from git import Repo
from git import Git


def main():
    git_ssh_identity_file = os.path.expanduser(last_version.id_rsa_path)
    git_ssh_cmd = 'ssh -i %s' % git_ssh_identity_file
    with Git().custom_environment(GIT_SSH_COMMAND=git_ssh_cmd):
        repo = Repo(last_version.sofi_plc_project_path)
        assert not repo.bare
        tags_local = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        origin = repo.remotes.origin
        origin.fetch()
        tags_remote = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        last_version.delete_remote_tags_from_local(repo, tags_local, tags_remote)
        version = tags_local[-1]
    print(version)
    return version


if __name__ == "__main__":
    main()
