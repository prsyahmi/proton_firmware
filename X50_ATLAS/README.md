# X50 ATLAS

## How does Init script work?

/bin/update.sh is executed.  
In this script, it will try to execute `/mnt/media_rw/xxx/b832bc61472727635baffcf25dd28e9f239273e2/{{md5hash}}.sh` where md5hash is:
- MD5 of the file content
- Result of previous MD5 + ro.build.product
- Result of previous MD5 + `Super Savior Hero`
- Go to Click Here → [recipe](https://gchq.github.io/CyberChef/#recipe=MD5()Find_/_Replace(%7B'option':'Regex','string':'$'%7D,'sx11ra_ds_2wk',false,false,false,false)MD5()Find_/_Replace(%7B'option':'Regex','string':'$'%7D,'Super%20Savior%20Hero',false,false,false,false)MD5()Find_/_Replace(%7B'option':'Regex','string':'$'%7D,'.sh',false,false,false,false)&input=IyEvc3lzdGVtL2Jpbi9zaAplY2hvICJBc3NhbGFtdWFsYWlrdW0sIFNlbGFtYXQgRGF0YW5nISI) ← Click Here

### Example:
ro.build.product = `sx11ra_ds_2wk`  
This can be obtained in `/system/build.prop`

| Content | MD5 |
|---------|-----|
|`echo HELLO WORLD!`|be25a89009d413ae073cff80a4fbd9be|
|`be25a89009d413ae073cff80a4fbd9besx11ra_ds_2wk`|6fcdbfad9eff73332903df03576a727f|
| `6fcdbfad9eff73332903df03576a727fSuper Savior Hero` | 68f054a674cdc7fa937bf1cfc9438608
|Final filename should be:|`68f054a674cdc7fa937bf1cfc9438608.sh` |

#### ro.build.product:
- `sx11ra_ds_2wk` = .617 / .618
- `sx11ra_qj_2wk` = .598

## Offline Upgrade/Downgrade
The ability to offline upgrade/downgrade will not be shared here.

## Protection
ATLAS OS has APK signature protection in package manager service where if the APK is not signed by them, it will refuse installation.

Around 1 March 2023, ACOTech decided to silently remove init script backdoor without bumping software version.