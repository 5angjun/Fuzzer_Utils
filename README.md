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
