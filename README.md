# Fuzzer_Utils
Fuzzer Utils to save times


# How to use


```
for count in {01..23}; do cp ./a.out ~/xpdf-4.04/xpdf/out_pdfinfo/fuzzer${count}/crashes; done
for count in {01..23}; do cd ~/xpdf-4.04/xpdf/out_pdfinfo/fuzzer${count}/crashes/; ~/xpdf-4.04/xpdf/out_pdfinfo/fuzzer${count}/crashes/a.out ${count}; done
```

```
cat /root/gdb_log
```
<p>[##_Image|kage@d891No/btrFFPjqOoa/xv5Zv7CBjRMvsRykhQ3xQ0/img.png|CDM|1.3|{"originWidth":1920,"originHeight":1080,"style":"alignCenter","filename":"Screenshot from 2022-06-24 15-24-49.png"}_##]</p>
