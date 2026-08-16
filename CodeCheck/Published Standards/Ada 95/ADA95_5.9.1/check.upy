ERR1 = "Isolate the use of Ada.Unchecked_Conversion in package bodies"
ERR2 = "Check the validity of scalar data produced by Ada.Unchecked_Conversion with the 'Valid attribute"


def ids():
    return ("ADA95_5.9.1",)


def name(id):
    return {
        "ADA95_5.9.1": "Published Standards/Ada 95/5.9.1 Unchecked Conversion",
    }[id]


def tags(id):
    return {
        "ADA95_5.9.1": [
            "Language: Ada",
            "Standard: Ada 95",
            "Programming Practices",
        ],
    }.get(id, [])


def detailed_description(id):
    return {
        "ADA95_5.9.1": """
<p><b>Guideline</b></p>
<p>&#8226; Use <code>Ada.Unchecked_Conversion</code> only with the utmost care (Ada
Reference Manual, &sect;13.9).</p>
<p>&#8226; Consider using the <code>'Valid</code> attribute to check the validity of
scalar data.</p>
<p>&#8226; Ensure that the value resulting from <code>Ada.Unchecked_Conversion</code>
properly represents a value of the parameter's subtype.</p>
<p>&#8226; Isolate the use of <code>Ada.Unchecked_Conversion</code> in package
bodies.</p>

<p><b>Example</b></p>
<p>The following example shows how to use the <code>'Valid</code> attribute
to check validity of scalar data:</p>
<pre><code language="Ada">with Ada.Unchecked_Conversion;
with Ada.Text_IO;
with Ada.Integer_Text_IO;

procedure Test is

   type Color is (Red, Yellow, Blue);
   for Color'Size use Integer'Size;

   function Integer_To_Color is
      new Ada.Unchecked_Conversion (Source => Integer,
                                    Target => Color);

   Possible_Color : Color;
   Number         : Integer;

begin  -- Test

   Ada.Integer_Text_IO.Get (Number);
   Possible_Color := Integer_To_Color (Number);

   if Possible_Color'Valid then
      Ada.Text_IO.Put_Line(Color'Image(Possible_Color));
   else
      Ada.Text_IO.Put_Line("Number does not correspond to a color.");
   end if;

end Test;
</code></pre>

<p><b>Rationale</b></p>
<p>An unchecked conversion is a bit-for-bit copy without regard to the
meanings attached to those bits and bit positions by either the source
or the destination type. The source bit pattern can easily be
meaningless in the context of the destination type. Unchecked
conversions can create values that violate type constraints on
subsequent operations. Unchecked conversion of objects mismatched in
size has implementation-dependent results.</p>
<p>Using the <code>'Valid</code> attribute on scalar data allows you to check whether
it is in range without raising an exception if it is out of range. There
are several cases where such a validity check enhances the readability
and maintainability of the code:</p>
<ul>
<li>Data produced through an unchecked conversion</li>
<li>Input data</li>
<li>Parameter values returned from a foreign language interface</li>
<li>Aborted assignment (during asynchronous transfer of control or
execution of an abort statement)</li>
<li>Disrupted assignment from failure of a language-defined check</li>
<li>Data whose address has been specified with the <code>'Address</code> attribute</li>
</ul>
<p>An access value should not be assumed to be correct when obtained
without compiler or run-time checks. When dealing with access values,
use of the <code>'Valid</code> attribute helps prevent the erroneous dereferencing
that might occur after using <code>Ada.Unchecked_Deallocation</code>,
<code>Unchecked_Access</code>, or <code>Ada.Unchecked_Conversion</code>.</p>
<p>In the case of a nonscalar object used as an actual parameter in an
unchecked conversion, you should ensure that its value on return from
the procedure properly represents a value in the subtype. This case
occurs when the parameter is of mode <code>out</code> or <code>in out</code>. It is important
to check the value when interfacing to foreign languages or using a
language-defined input procedure. The Ada Reference Manual,
&sect;13.9.1 lists the full rules
concerning data validity.</p>

<p><b>Developer's Note</b></p>
<p>Two of the four guideline bullets are automated: isolating an
<code>Ada.Unchecked_Conversion</code> instantiation (or a rename of one) to a
package body, and checking the <code>'Valid</code> attribute on data produced by an
instantiation whose <code>Target</code> is a scalar type. "Use only with the utmost
care" is a judgment call with no structural signature and is not automatable.
"Ensure the resulting value properly represents a value of the subtype" is
not checked as a separate rule; it is the outcome the <code>'Valid</code> check
exists to guarantee, so it is covered by that same check.</p>
<p>The <code>'Valid</code> check is not flow-sensitive: it flags a converted value
only if it is never checked with <code>'Valid</code> anywhere in scope, not whether
the check happens before the value is first used. A value used and only
later validated is not flagged.</p>
<p>Whether a <code>Target</code> type counts as scalar is based on Understand's
reported entity kind, excluding record, array, access, task, protected, and
interface types. A private type with no further category token in its kind
(e.g. <code>System.Address</code> in GNAT's runtime) is treated as scalar-eligible,
consistent with how a genuinely scalar private type is classified. This
matches the guideline's own rationale, which lists address data as a case
warranting a <code>'Valid</code> check, but it means an opaque private type whose
full view cannot be determined is treated the same as one that is
genuinely scalar.</p>
""",
    }[id]


def test_entity(file):
    return file.kind().check("Ada File")


def test_language(language):
    return language == "Ada"


_NON_SCALAR_KINDS = "Record, Array, Access, Task, Protected, Interface"


def _instantiates_unchecked_conversion(ent):
    for generic_ref in ent.refs("Instanceof"):
        if str(generic_ref.ent().longname()) == "Ada.Unchecked_Conversion":
            return True

    return False


def _resolves_to_unchecked_conversion(ent, seen_ids=None):
    if seen_ids is None:
        seen_ids = set()

    if ent.id() in seen_ids:
        return False

    seen_ids.add(ent.id())

    if _instantiates_unchecked_conversion(ent):
        return True

    for rename_ref in ent.refs("Rename"):
        if _resolves_to_unchecked_conversion(rename_ref.ent(), seen_ids):
            return True

    return False


def _target_type_ent(ent):
    type_refs = ent.refs("Typed")

    return type_refs[0].ent() if type_refs else None


def _is_scalar_type(type_ent):
    if type_ent is None:
        return False

    return not type_ent.kind().check(_NON_SCALAR_KINDS)


def _is_valid_attribute_use(file, lexer, use_ref):
    lex = lexer.lexeme(use_ref.line(), use_ref.column())

    if not lex:
        return False

    lex = lex.next()

    if not lex or lex.text() != "'":
        return False

    lex = lex.next()

    return bool(lex) and lex.text().lower() == "valid"


def _has_valid_check(obj_ent, file, lexer):
    for use_ref in obj_ent.refs("Useby Attribute"):
        if _is_valid_attribute_use(file, lexer, use_ref):
            return True

    return False


def check(check, file):
    is_spec_file = bool(file.filerefs("Declare Spec File"))
    lexer = file.lexer(lookup_ents=False)

    for decl_ref in file.filerefs("Declare", "Function, Alias"):
        ent = decl_ref.ent()

        if not _resolves_to_unchecked_conversion(ent):
            continue

        if is_spec_file:
            check.violation(ent, file, decl_ref.line(),
                            decl_ref.column(), ERR1)

        target_ent = _target_type_ent(ent)

        if not _is_scalar_type(target_ent):
            continue

        for assign_ref in ent.refs("Assignby Value"):
            obj_ent = assign_ref.ent()

            if not _has_valid_check(obj_ent, file, lexer):
                check.violation(
                    obj_ent, file, assign_ref.line(), assign_ref.column(), ERR2
                )
