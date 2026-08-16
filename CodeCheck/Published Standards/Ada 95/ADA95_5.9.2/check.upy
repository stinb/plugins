ERR1 = "Isolate the use of Ada.Unchecked_Deallocation in package bodies"


def ids():
    return ("ADA95_5.9.2",)


def name(id):
    return {
        "ADA95_5.9.2": "Published Standards/Ada 95/5.9.2 Unchecked Deallocation",
    }[id]


def tags(id):
    return {
        "ADA95_5.9.2": [
            "Language: Ada",
            "Standard: Ada 95",
            "Programming Practices",
        ],
    }.get(id, [])


def detailed_description(id):
    return {
        "ADA95_5.9.2": """
<p><b>Guideline</b></p>
<p>&#8226; Isolate the use of <code>Ada.Unchecked_Deallocation</code> in package
bodies.</p>
<p>&#8226; Ensure that no dangling reference to the local object exists after
exiting the scope of the local object.</p>

<p><b>Rationale</b></p>
<p>Most of the reasons for using <code>Ada.Unchecked_Deallocation</code> with caution
have been given in Guideline 5.4.5. When this feature is used, no
checking is performed to verify that there is only one access path to
the storage being deallocated. Thus, any other access paths are not made
null. Depending on the value of these other access paths could result
in erroneous execution.</p>
<p>If your Ada environment implicitly uses dynamic heap storage but does
not fully and reliably reclaim and reuse heap storage, you should not
use <code>Ada.Unchecked_Deallocation</code>.</p>

<p><b>Developer's Note</b></p>
<p>Isolating an <code>Ada.Unchecked_Deallocation</code> instantiation (or a rename
of one) to a package body is automated. "Ensure that no dangling reference
to the local object exists after exiting the scope of the local object" is
not automatable: it requires tracking every alias of a deallocated access
value across the program, which is an interprocedural data-flow problem, not
a structural pattern. <code>ADA95_5.4.5</code>'s Developer's Note already states
this same limitation for the same underlying problem.</p>
""",
    }[id]


def test_entity(file):
    return file.kind().check("Ada File")


def test_language(language):
    return language == "Ada"


def _instantiates_unchecked_deallocation(ent):
    for generic_ref in ent.refs("Instanceof"):
        if str(generic_ref.ent().longname()) == "Ada.Unchecked_Deallocation":
            return True

    return False


def _resolves_to_unchecked_deallocation(ent, seen_ids=None):
    if seen_ids is None:
        seen_ids = set()

    if ent.id() in seen_ids:
        return False

    seen_ids.add(ent.id())

    if _instantiates_unchecked_deallocation(ent):
        return True

    for rename_ref in ent.refs("Rename"):
        if _resolves_to_unchecked_deallocation(rename_ref.ent(), seen_ids):
            return True

    return False


def check(check, file):
    is_spec_file = bool(file.filerefs("Declare Spec File"))

    if not is_spec_file:
        return

    for decl_ref in file.filerefs("Declare", "Procedure, Alias"):
        ent = decl_ref.ent()

        if _resolves_to_unchecked_deallocation(ent):
            check.violation(ent, file, decl_ref.line(),
                            decl_ref.column(), ERR1)
