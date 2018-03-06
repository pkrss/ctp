package time

import "time"

// 交易日8：00-17：00，还有夜盘的20：00-02：30
func NowIsCtpWorkingTime() bool {
	t := time.Now()

	switch t.Weekday() {
	case time.Sunday, time.Saturday:
		return false
	}

	h := t.Hour()
	m := t.Minute()

	if h >= 8 && h < 17 {
		return true
	}

	if h >= 20 {
		return true
	}

	if h < 2 || ((h == 2) && (m < 30)) {
		return true
	}

	return false
}
