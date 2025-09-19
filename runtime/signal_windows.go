//go:build windows

package runtime

import (
	"os"
	"syscall"
)

var stopSignals = []os.Signal{syscall.SIGHUP, syscall.SIGINT, syscall.SIGTERM, syscall.SIGQUIT}
