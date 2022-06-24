# Fuzzer_Utils
Fuzzer Utils to save times

## 목적: 크래시 파일을 자동으로 gdb에 돌려 gdb결과를 파일에 저장

## 구현 : 부모프로세스와 자식 프로세스가 파이프 통신을 통해 자식의 stdout과 stderr을 기록한다.


# How to use


```
for count in {01..23}; do cp ./a.out ~/xpdf-4.04/xpdf/out_pdfinfo/fuzzer${count}/crashes; done
for count in {01..23}; do cd ~/xpdf-4.04/xpdf/out_pdfinfo/fuzzer${count}/crashes/; ~/xpdf-4.04/xpdf/out_pdfinfo/fuzzer${count}/crashes/a.out ${count}; done
```

```
cat /root/gdb_log
```
![ex_screenshot](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2Fd891No%2FbtrFFPjqOoa%2Fxv5Zv7CBjRMvsRykhQ3xQ0%2Fimg.png)
