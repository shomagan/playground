import os
from git import Repo
from git import Git
id_rsa_path = 'C:/Users/shoma/.ssh/id_rsa'
sofi_plc_project_path = 'D:/_work/sofi_plc'
plc_ip_address = '192.168.1.236'


def compare_version(local, remote):
    """return 1 if last version larger them current or 0"""
    result = 0
    if len(local) and len(remote):
        current = local.replace("v", "")
        current = [int(numeric_string) for numeric_string in current.split(".")]
        last = remote.replace("v", "")
        last = [int(numeric_string) for numeric_string in last.split(".")]
        if len(current) >= 4 and len(last) >= 4:
            if current[0] < last[0]:
                result = 1
            elif current[0] == last[0]:
                if current[1] < last[1]:
                    result = 1
                elif current[1] == last[1]:
                    if current[2] < last[2]:
                        result = 1
                    elif current[2] == last[2]:
                        if current[3] < last[3]:
                            result = 1
        else:
            print('version len mismatch {} {}'.format(len(current), len(last)))
    else:
        print('version did not get before')
    return result


def delete_remote_tags_from_local(repository, tags_local, tags_remote):
    if len(tags_local) and len(tags_remote):
        for i in range(1, len(tags_local)):
            if compare_version(str(tags_local[-1]), str(tags_remote[-i])):
                repository.delete_tag(tags_remote[-i])
            else:
                break


def delete_unpaired_tags_from_local(repo, tags_local, last_tag):
    if len(tags_local):
        for i in range(1, len(tags_local)):
            if compare_version(str(last_tag), str(tags_local[-i])):
                repo.delete_tag(tags_local[-i])
            else:
                break


def main():
    git_ssh_identity_file = os.path.expanduser(id_rsa_path)
    git_ssh_cmd = 'ssh -i %s' % git_ssh_identity_file
    with Git().custom_environment(GIT_SSH_COMMAND=git_ssh_cmd):
        repo = Repo(sofi_plc_project_path)
        assert not repo.bare
        tags_local_main = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        origin = repo.remotes.origin
        origin.fetch()
        tags_remote_main = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        remote_version_sofi_main = str(tags_remote_main[-1])
        delete_remote_tags_from_local(repo, tags_local_main, tags_remote_main)
        repo = Repo(sofi_plc_project_path + '/free_rtos/canfestival')
        assert not repo.bare
        tags_local_can = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        origin = repo.remotes.origin
        origin.fetch()
        tags_remote_can = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        remote_version_sofi_can = str(tags_remote_can[-1])
        delete_remote_tags_from_local(repo, tags_local_can, tags_remote_can)
        repo = Repo(sofi_plc_project_path + '/free_rtos/tools/generators')
        assert not repo.bare
        tags_local_generators = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        origin = repo.remotes.origin
        origin.fetch()
        tags_remote_generators = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        remote_version_sofi_generators = str(tags_remote_generators[-1])
        delete_remote_tags_from_local(repo, tags_local_generators, tags_remote_generators)
        repo = Repo(sofi_plc_project_path + '/free_rtos/docs')
        assert not repo.bare
        tags_local_docs = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        origin = repo.remotes.origin
        origin.fetch()
        tags_remote_docs = sorted(repo.tags, key=lambda t: t.commit.committed_datetime)
        remote_version_sofi_docs = str(tags_remote_docs[-1])
        delete_remote_tags_from_local(repo, tags_local_docs, tags_remote_docs)
        print('last version sofi main - {}\n'
              'last version canfestival - {}\n'
              'last version generators - {}\n'
              'last version docs - {}'.format(remote_version_sofi_main, remote_version_sofi_can,
                                              remote_version_sofi_generators, remote_version_sofi_docs))
    return ('last version sofi main - {}\n'
            'last version canfestival - {}\n'
            'last version generators - {}\n'
            'last version docs - {}'.format(remote_version_sofi_main, remote_version_sofi_can,
                                            remote_version_sofi_generators, remote_version_sofi_docs))


if __name__ == "__main__":
    main()
