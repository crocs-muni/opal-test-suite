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

>[!NOTE]
> The set of tests is not final, further test scenarios are in development.

## Tests
Tests are realized as a set of Bash scripts with assisting C programs. Each of them is to be run individually.

Following tests are currently available:
- `scenario_basic` - a set of basic drive operations to verify the most essential Opal features,
- `test_vuln_list` - a check whether the model of a given drive is mentioned in known security reports,
- `test_rng_quality` - a statistical test of randomness provided by the drive,
- `test_lbafs` - a test of LBA size information reporting consistency.

## Usage
To run the Opal Test Suite, clone this repository and run individual desired test scripts with root privileges.

Tests with a `scenario` prefix are configured within `.config`. To run such tests, the following suffices:

```bash
./scenario_basic
```

Tests with a `test` prefix currently do not use the present configuration file but still need specific variables to be set. For example, to run `test_rng_quality`, the script can be run in following way:

```bash
DEV=/dev/<your device> ./test_rng_quality
```