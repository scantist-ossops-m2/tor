#!/bin/bash

# This script is used to build Tor for continuous integration.  It should
# be kept the same for all supported Tor versions.
#
# It's subject to the regular Tor license; see LICENSE for copying
# information.

set -o errexit
set -o nounset

# Options for this script.
DEBUG_CI="${DEBUG_CI:-no}"
COLOR_CI="${COLOR_CI:-yes}"

# Options for which CI system this is.
ON_GITLAB="${ON_GITLAB:-yes}"

# Options for how to build Tor.  All should be yes/no.
FATAL_WARNINGS="${FATAL_WARNINGS:-yes}"
HARDENING="${HARDENING:-no}"
COVERAGE="${COVERAGE:-no}"
RUST="${RUST:-no}"
DOXYGEN="${DOXYGEN:-no}"
ASCIIDOC="${ASCIIDOC:-no}"

# Options for which tests to run.   All should be yes/no.
CHECK="${CHECK:-yes}"
STEM="${STEM:-no}"
CHUTNEY="${CHUTNEY:-no}"
DISTCHECK="${DISTCHECK:-no}"

# Options for where the Tor source is.
CI_SRCDIR="${CI_SRCDIR:-.}"

# Options for where to build.
CI_BUILDDIR="${CI_BUILDDIR:-./build}"

# How parallel should we run make?
MAKE_J_OPT="${MAKE_J_OPT:--j4}"
# Should we stop after make finds an error?
MAKE_K_OPT="${MAKE_K_OPT:--k}"

# What make target should we use for chutney?
CHUTNEY_MAKE_TARGET="${CHUTNEY_MAKE_TARGET:-test-network}"

# Where do we find our additional testing tools?
CHUTNEY_PATH="${CHUTNEY_PATH:-}"
STEM_PATH="${STEM_PATH:-}"

#############################################################################
# Preliminary functions.

# Terminal coloring/emphasis stuff.
if [[ "${COLOR_CI}" == "yes" ]]; then
    T_RED=$(tput setaf 1 || true)
    T_GREEN=$(tput setaf 2 || true)
    T_DIM=$(tput dim || true)
    T_BOLD=$(tput bold || true)
    T_RESET=$(tput sgr0 || true)
else
    T_RED=
    T_GREEN=
    T_DIM=
    T_BOLD=
    T_RESET=
fi

function error()
{
    echo "${T_BOLD}${T_RED}ERROR:${T_RESET} $*" 1>&2
}
function die()
{
    echo "${T_BOLD}${T_RED}FATAL ERROR:${T_RESET} $*" 1>&2
    exit 1
}
function hooray()
{
    echo "${T_BOLD}${T_GREEN}$*${T_RESET}"
}

if [[ "${DEBUG_CI}" == "yes" ]]; then
    function debug()
    {
        echo "${T_DIM}(debug): $*${T_RESET}"
    }
else
    function debug()
    {
        :
    }
fi

function yes_or_no()
{
    local varname="$1"
    local value="${!varname}"
    debug "${varname} is ${value}"
    if [[ "${value}" != 'yes' && "${value}" != 'no' ]]; then
        die "${varname} must be 'yes' or 'no'.  Got unexpected value ${value}".
    fi
}

function incompatible()
{
    local varname1="$1"
    local varname2="$2"
    local val1="${!varname1}"
    local val2="${!varname2}"
    if [[ "${val1}" = 'yes' && "${val2}" = 'yes' ]]; then
        die "Cannot set both ${varname1} and ${varname2}: they are incompatible."
    fi
}

function runcmd()
{
    echo "${T_BOLD}\$ $*${T_RESET}"
    if ! "$@" ; then
        error "command '$*' has failed."
        return 1
    fi
}

function show_git_version()
{
    local tool="$1"
    local dir="$2"
    local version="?????"
    if [[ -e "$dir/.git" ]] ; then
        version=$(cd "$dir"; git rev-parse HEAD)
    fi
    echo "${T_BOLD}$tool:${T_RESET} $version"
}

if [[ "${ON_GITLAB}" == "yes" ]]; then
    function start_section()
    {
	local label="$1"
	local stamp
	stamp=$(date +%s)
	printf "section_start:%s:%s\r\e[0K" "$stamp" "$label"
	echo "${T_BOLD}${T_GREEN}========= $label${T_RESET}"
    }
    function end_section()
    {
	local label="$1"
	local stamp
	stamp=$(date +%s)
	printf "section_end:%s:%s\r\e[0K" "$stamp" "$label"
    }
else
    function start_section()
    {
	true
    }
    function end_section()
    {
	true
    }
fi

if [[ "$*" == "" ]]; then
    RUN_STAGE_CONFIGURE="yes"
    RUN_STAGE_BUILD="yes"
    RUN_STAGE_TEST="yes"
else
    RUN_STAGE_CONFIGURE="no"
    RUN_STAGE_BUILD="no"
    RUN_STAGE_TEST="no"

    for stage in "$@"; do
	case "$stage" in
	    configure)
		RUN_STAGE_CONFIGURE="yes"
		;;
	    build)
		RUN_STAGE_BUILD="yes"
		;;
	    test)
		RUN_STAGE_TEST="yes"
		;;
	    *)
		error "Unknown stage $stage"
		;;
	esac
    done
fi

#############################################################################
# Validate inputs.

debug Validating inputs
yes_or_no DEBUG_CI
yes_or_no COLOR_CI
yes_or_no ON_GITLAB
yes_or_no FATAL_WARNINGS
yes_or_no HARDENING
yes_or_no COVERAGE
yes_or_no RUST
yes_or_no DOXYGEN
yes_or_no ASCIIDOC

yes_or_no CHECK
yes_or_no STEM
yes_or_no DISTCHECK

incompatible DISTCHECK CHECK
incompatible DISTCHECK CHUTNEY
incompatible DISTCHECK STEM
incompatible DISTCHECK COVERAGE
incompatible DISTCHECK DOXYGEN

if [[ "${CHUTNEY}" = yes && "${CHUTNEY_PATH}" = '' ]] ; then
    die "CHUTNEY is set to 'yes', but CHUTNEY_PATH was not specified."
fi

if [[ "${STEM}" = yes && "${STEM_PATH}" = '' ]] ; then
    die "STEM is set to 'yes', but STEM_PATH was not specified."
fi

#############################################################################
# Set up options for make and configure.

make_options=()
if [[ "$MAKE_J_OPT" != "" ]]; then
    make_options+=("$MAKE_J_OPT")
fi
if [[ "$MAKE_K_OPT" != "" ]]; then
    make_options+=("$MAKE_K_OPT")
fi

configure_options=()
if [[ "$FATAL_WARNINGS" == "yes" ]]; then
    configure_options+=("--enable-fatal-warnings")
fi
if [[ "$HARDENING" == "yes" ]]; then
    configure_options+=("--enable-fragile-hardening")
fi
if [[ "$COVERAGE" == "yes" ]]; then
    configure_options+=("--enable-coverage")
fi
if [[ "$RUST" == "yes" ]]; then
    configure_options+=("--enable-rust")
fi
if [[ "$ASCIIDOC" != "yes" ]]; then
    configure_options+=("--disable-asciidoc")
fi

#############################################################################
# Tell the user about our versions of different tools and packages.

uname -a
python -V || echo "no 'python' binary."
python3 -V || echo "no 'pythone' binary."

show_git_version Tor "${CI_SRCDIR}"
if [[ "${STEM}" = "yes" ]]; then
    show_git_version Stem "${STEM_PATH}"
fi
if [[ "${CHUTNEY}" = "yes" ]]; then
    show_git_version Chutney "${CHUTNEY_PATH}"
fi

#############################################################################
# Make sure the directories are all there.

# Make sure CI_SRCDIR exists and has a file we expect.
if [[ ! -d "$CI_SRCDIR" ]] ; then
    die "CI_SRCDIR=${CI_SRCDIR} is not a directory"
fi
if [[ ! -f "$CI_SRCDIR/src/core/or/or.h" ]] ; then
    die "CI_SRCDIR=${CI_SRCDIR} does not look like a Tor directory."
fi

# Make CI_SRCDIR absolute.
CI_SRCDIR=$(cd "$CI_SRCDIR" && pwd)

# Create an "artifacts" directory to copy artifacts into.
mkdir -p ./artifacts

if [[ "$RUN_STAGE_CONFIGURE" = "yes" ]]; then

    start_section "Autogen"
    runcmd cd "${CI_SRCDIR}"
    runcmd ./autogen.sh
    runcmd mkdir -p "${CI_BUILDDIR}"
    runcmd cd "${CI_BUILDDIR}"
    end_section "Autogen"

    # make the builddir absolute too.
    CI_BUILDDIR=$(pwd)

    start_section "Configure"
    if ! runcmd "${CI_SRCDIR}"/configure "${configure_options[@]}" ; then
	error "Here is the end of config.log:"
	runcmd tail config.log
	die "Unable to continue"
    fi
    end_section "Configure"
else
    debug "Skipping configure stage. Making sure that ${CI_BUILDDIR}/config.log exists."
    if [[ ! -d "${CI_BUILDDIR}" ]]; then
	die "Build directory ${CI_BUILDDIR} did not exist!";
    fi
    if [[ ! -f "${CI_BUILDDIR}/config.log" ]]; then
	die "Tor was not configured in ${CI_BUILDDIR}!";
    fi

    cp config.log "${CI_SRCDIR}"/artifacts

    runcmd cd "${CI_BUILDDIR}"
    CI_BUILDDIR=$(pwd)
fi

###############################
# Build Tor.

if [[ "$RUN_STAGE_BUILD" = "yes" ]] ; then
    if [[ "$DISTCHECK" = "no" ]]; then
	start_section "Build"
	runcmd make "${make_options[@]}" all
        cp src/app/tor "${CI_SRCDIR}"/artifacts
	end_section "Build"
    else
	export DISTCHECK_CONFIGURE_FLAGS="${configure_options[*]}"
	# XXXX Set make options?
	start_section Distcheck
	if runcmd make "${make_options[@]}" distcheck ; then
            hooray "Distcheck was successful. Nothing further will be done."
            # We have to exit early here, since we can't do any other tests.
            cp tor-*.tar.gz "${CI_SRCDIR}"/artifacts
            exit 0
	else
            error "Diagnostics:"
            runcmd make show-distdir-testlog || true
            runcmd make show-distdir-core || true
            die "Unable to continue."
	fi
	end_section Distcheck
    fi
fi
##############################
# Run tests.

if [[ "$RUN_STAGE_TEST" == "no" ]]; then
    echo "Skipping tests. Exiting now."
    exit 0
fi

if [[ "$RUN_STAGE_BUILD" = "no" ]] ; then
    debug "Skipped build stage. Making sure that ./src/app/tor exists."
    if [[ ! -f "./src/app/tor" ]]; then
	die "$(pwd)/src/app/tor does not exist"
    fi
fi

FAILED_TESTS=""

if [[ "${DOXYGEN}" = 'yes' ]]; then
    start_section Doxygen
    if runcmd make doxygen; then
	hooray "make doxygen has succeeded."
    else
	FAILED_TESTS="${FAILED_TESTS} doxygen"
    fi
    end_section Doxygen
fi

if [[ "${CHECK}" = "yes" ]]; then
    start_section "Check"
    if runcmd make "${make_options[@]}" check; then
        hooray "make check has succeeded."
    else
        error "Here are the contents of the test suite output:"
        runcmd cat test_suite.log || true
        FAILED_TESTS="${FAILED_TESTS} check"
    fi
    end_section "Check"
fi

if [[ "${CHUTNEY}" = "yes" ]]; then
    start_section "Chutney"
    if runcmd make "${CHUTNEY_MAKE_TARGET}"; then
        hooray "Chutney tests have succeeded"
    else
        error "Chutney says:"
        runcmd "${CHUTNEY_PATH}"/tools/diagnostics.sh || true
        # XXXX These next two should be part of a make target.
        runcmd ls test_network_log || true
        runcmd cat test_network_log || true
        FAILED_TESTS="${FAILED_TESTS} chutney"
    fi
    end_section "Chutney"
fi

if [[ "${STEM}" = "yes" ]]; then
   start_section "Stem"
   # XXXX This shold probably be part some test-stem make target.
   if runcmd timelimit -p -t 520 -s USR1 -T 30 -S ABRT \
         python3 "${STEM_PATH}/run_tests.py" \
         --tor src/app/tor \
         --integ --test control.controller \
         --test control.base_controller \
         --test process \
         --log TRACE \
         --log-file stem.log ; then
       hooray "Stem tests have succeeded"
   else
       error "Stem output:"
       runcmd tail -1000 "${STEM_PATH}"/test/data/tor_log
       runcmd grep -v "SocketClosed" stem.log | tail -1000
       FAILED_TESTS="${FAILED_TESTS} stem"
   fi
   end_section "Stem"
fi

# TODO: Coverage

if [[ "${FAILED_TESTS}" != "" ]]; then
    die "Failed tests: ${FAILED_TESTS}"
fi

hooray "Everything seems fine."
