//go:build windows

package core

import (
	"os"
	"syscall"
)

var stopSignals = []os.Signal{syscall.SIGHUP, syscall.SIGINT, syscall.SIGTERM, syscall.SIGQUIT}
