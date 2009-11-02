<?php include("header.php"); ?>


<div id="content">


<h3>Notes</h3>

<p><strong>New to the Mac?</strong> Here's a guide that might help from <a href="http://arstechnica.com/guides/tweaks/miniguide.ars">Ars Technica</a>. And you may wish to know what those symbols in the menus mean:<br />
<img src="command-key-symbol.gif" border="0" alt="Command Key Symbol" /> is the Command (or Apple) key located next to the space bar on both sides on most Mac keyboards.<br />
<img src="option-key-symbol.gif" border="0" alt="Option Key Symbol" /> is the Option (or Alt) key located next to the Command keys on both sides on most Mac keyboards.<br />
<img src="control-key-symbol.gif" border="0" alt="Control Key Symbol" /> is the Control key located next to the Option keys on both sides on most Mac keyboards.<br />
<img src="shift-key-symbol.gif" border="0" alt="Shift Key Symbol" /> is the Shift key located above the Control key on both sides on most Mac keyboards.<br />
</p>
<p>Need some information on keyboard shortcuts for Mac OS? Try these links: <a href="http://docs.info.apple.com/article.html?artnum=75459">Keyboard shortcuts</a>, and <a href="http://developer.apple.com/documentation/UserExperience/Conceptual/OSXHIGuidelines/XHIGKeyboardShortcuts/chapter_12_section_1.html">Developer Documentation</a>.
</p>
<p>For an alternative forward delete, try <i><font color="#ff9922">fn</font></i> delete.
</p>

<p>If you just want to swap a couple of standard keys, you might want to look at <a href="http://developer.apple.com/technotes/tn2002/tn2056.html" class="navigation">http://developer.apple.com/technotes/tn2002/tn2056.html</a> for the details.
</p>


<p>You may experience problems using a key like enter as a modifier when dragging with the trackpad if you have "ignore trackpad when typing" turned on. You can simply go to the Keyboard preference pane and turn this feature off if you have this trouble.
</p>

<p>
<i>
If you want the extra Command key feature under Classic Mac OS (8 or 9) you could go here:
<a href="http://www.resexcellence.com/hack_html_99/07-02-99.shtml">www.resexcellence.com/hack_html_99/07-02-99.shtml</a>
or here: <a href="http://www.macmilitia.com/how-to/ibookhack.shtml">www.macmilitia.com/how-to/ibookhack.shtml</a>
for resedit hacks, or here:
<a href="http://opus.funstaff.ch/freeware/sk/">opus.funstaff.ch/freeware/sk/</a> for more friendly software.
</i>
</p>




<h3>Installing DoubleCommand</h3>
<p>To install DoubleCommand, please perform the following steps:</p>
<p>1.	Remove any versions of iJect, uControl, CommandandControl or PowerDelete (see below for how to do that). These are conflicting software. Note that you do not have to remove old versions of DoubleCommand anymore.</p>
<p>2. Open the disk image and open the installer package within.
</p>

<h3>Configuring DoubleCommand</h3>
<p>
DoubleCommand now includes a preference pane, so just open up System Preferences and look for DoubleCommand.
</p>

<h3>Removing DoubleCommand</h3>
<p>To remove DoubleCommand from your machine, open the Terminal and paste in the following: <span class="code">/Library/StartupItems/DoubleCommand/uninstall.command</span></p>

<h3>Removing conflicting software</h3>
<p>To remove conflicting software from your machine:</p>
<p>1.	Delete the iJect, uControl, CommandandControl or PowerDelete folder in /Library/StartupItems on your Mac OS X startup volume.</p>
<p>2.	Restart your computer.</p>

<h3>Troubleshooting</h3>
<p>
If for some reason DoubleCommand crashes, it automatically disables itself. To try loading again, just remove the DoubleCommand.crashed file from /var/tmp.
</p>

<p>DoubleCommand has been developed for Mac OS X. It is known to run on Mac OS X 10.1 through to Mac OS X 10.6 (using the correct versions for each system) and probably runs on later versions, too. However, since it links to internal kernel structures, newer versions of the kernel may break DoubleCommand. After failing to load for any reason, DoubleCommand automatically deactivates itself to avoid continuing problems. If this happens check with this web site for an update that will work with the new kernel.</p>
<p>You can start the computer while holding down Command and S to get single user mode. While in this mode you can type the following to disable DoubleCommand:
<br /><br />
<span class="code">
fsck -y<br />
mount -uw /<br />
cd /Library/StartupItems/DoubleCommand<br />
mv DoubleCommand DoubleCommand.disabled<br />
shutdown -r now<br />
</span>
<br /><br />
When you restart the computer DoubleCommand will no longer be active.
</p>



</div>


<?php include("footer.php"); ?>
