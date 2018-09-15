#include <common/misc.S>

<?
	$file_contents = file_get_contents($_ENV["PATH_TO_DEF"]);
	$fake_functions = explode(",", $_ENV["FAKE_FUNCS"]);

	$all_functions_ = explode("\r\n", $file_contents);
	array_shift($all_functions_);
	array_shift($all_functions_);
	$all_functions = array();
	# $passthrough_functions = array();
	foreach($all_functions_ as $function) {
		$parts = explode("\t", $function);
		$first_part = $parts[1];
		$first_part_pre_equals = explode("=", $first_part)[0];
		array_push($all_functions, $first_part_pre_equals);
	}
?>

.data

.globl RealDll
RealDll: LONG_OR_QUAD 0


<? foreach($all_functions as $function) { ?> GLOBL_LABEL(Real_<?=$function?>): LONG_OR_QUAD 0
<? } ?>

.section .rdata,"dr"
	.globl REAL_DLL_PATH;
	REAL_DLL_PATH:	.ascii "<?= $_ENV["REAL_DLL_FIXED_PATH"] ?>\0"
	.FatalErrorMsg:	.ascii "Fatal error!\0"

	<? foreach($all_functions as $function) { ?>.str_<?= $function ?>: .ascii "<?= $function ?>\0"
	<? } ?>

# end .data

.text

# Define and export each symbol in the def
# Load the Real_ versions of everything
# Jump to either Fake_* or Real* as appropriate
# TODO investigate thread safety

<? include "_x64.php.S" ?>
<? include "_x32.php.S" ?>

.section .drectve

<? foreach($all_functions as $function) { ?>
	# .ascii " -export:\"<?=$function?>\""
<? } ?>
