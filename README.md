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
![ex_screenshot](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2Fd891No%2FbtrFFPjqOoa%2Fxv5Zv7CBjRMvsRykhQ3xQ0%2Fimg.png)
