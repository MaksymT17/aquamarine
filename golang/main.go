package main

import (
    "log"
    "os"
    "os/exec"
)

func main() {
    arg0 := "../scripts/directory_scan_and_compare.py"
    arg1 := "data/"
    arg2 := "configuration.csv"
    arg3 := "output/"
    cmd := exec.Command("python3", arg0, arg1, arg2, arg3)
  //  cmd := exec.Command("ll")
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    log.Println(cmd.Run())
}
