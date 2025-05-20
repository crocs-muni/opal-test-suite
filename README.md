# Opal Test Suite

A test suite for behavior analysis of drives implementing the Opal hardware encryption standard by the Trusted Computing Group (TCG).

This tool provides assurance about conformance with the Opal specification and other drive traits related to secure data storage. Any test-reported warnings or errors suggest that a given drive might not provide the desired level of data protection and the user should be careful, if not reluctant, to use the drive for storing sensitive or valuable information.

For detailed information on the TCG Opal implementation see
- [TCG Storage Architecture Core Specification](https://trustedcomputinggroup.org/resource/tcg-storage-architecture-core-specification/),
- [TCG Storage Security Subsystem Class: Opal Specification](https://trustedcomputinggroup.org/resource/storage-work-group-storage-security-subsystem-class-opal/).

The Opal Test Suite utilizes [opal-toolset](https://github.com/crocs-muni/opal-toolset) tools to communicate with given drives and trigger Opal methods.

> [!CAUTION]
> **Backup data stored on the drive before running any tests of Opal Test Suite!**
>
> Tests of the suite are destructive and data stored on the drive might be lost.

## Tests
Tests are realized as a set of Bash scripts with assisting C programs. Each of them is to be run individually.

Following tests are currently available:
- `info` - the only non-destructive test gathering essential metadata and checking presence of mandatory Opal features,
- `test_basic` - a set of basic drive operations to verify the most essential Opal operations,
- `test_lr_defs` - an extensive test verifying definition of multiple locking ranges per user, their locking and read/write access control
- `test_vuln_list` - a check whether the model of a given drive is mentioned in known security reports,
- `test_rng_quality` - a statistical randomness test of random data provided by the drive,
- `test_rekey_patterns` - a look-up of potential encryption patterns,
- `test_psid_suffix` - a test for checking acceptance of PSID password with a suffix,
- `test_lbafs` - a test of LBA size information reporting consistency,
- `test_sum` - a test of support and behavior of the Single User Mode Feature set.

## Usage
To run the Opal Test Suite, clone this repository and run individual desired test scripts with root privileges.

Tests with a `test_*` prefix, including `info`, belong to the set of available tests. Remaining files represent helper utilities and outsourced code.

Tests with a `test` prefix need specific variables to be set. Individual tests are run as follows:

```bash
DEV=<dev> ./info

DEV=<dev> PSID=<psid> ./test_basic

DEV=<dev> ./test_rng_quality

DEV=<dev> PSID=<psid> ./test_lr_defs

DEV=<dev> PSID=<psid> ./test_lbafs

DEV=<dev> PSID=<psid> ./test_rekey_patterns

DEV=<dev> PSID=<psid> ./test_single_user_mode

DEV=<dev> PSID=<psid> ./test_psid_suffix

DEV=<dev> ./test_vuln_list
```

If you wish to see detailed information on the reported or deduced Opal minor version of your drive, run the following utility:

```bash
DEV=<dev> ./get_opal_minor_version 
```
