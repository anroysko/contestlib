struct FastIO {
        private:
                const static int K = 1 << 15;
                array<char, K> in_buf, out_buf;
                int in_fd, out_fd, in_x = K, out_x = 0;

                void refillBuffer() {
                        if (in_x < K) {
                                memmove(in_buf.data(), in_buf.data() + in_x, K - in_x);
                                in_x = K - in_x;
                        } else in_x = 0;
                        read(in_fd, in_buf.data() + in_x, K - in_x);
                        in_x = 0;
                }
                void flush() {
                        if (out_x) write(out_fd, out_buf.data(), out_x);
                        out_x = 0;
                }
        public:
                FastIO(FILE* in = stdin, FILE* out = stdout) : in_fd(fileno(in)), out_fd(fileno(out)) {}
                ~FastIO() { flush(); close(out_fd); }

                int readInt() {
                        if (in_x >= K / 2) refillBuffer();
                        while(in_buf[in_x] < '-') ++in_x;

                        int res = 0, sg = 1;
                        if (in_buf[in_x] == '-') sg = -1, ++in_x;
                        for (; in_buf[in_x] > '-'; ++in_x) res = (10 * res + (in_buf[in_x] - '0'));
                        return sg * res;
                }
                void printChar(char c) {
                        out_buf[out_x] = c;
                        ++out_x;
                        if (out_x >= K / 2) flush();
                }
                void printInt(int num) {
                        if (num == 0) {
                                out_buf[out_x] = '0';
                                ++out_x;
                        } else if (num < 0) {
                                out_buf[out_x] = '-';
                                ++out_x;
                        }
                        int s = out_x;
                        for (; num > 0; num /= 10, ++out_x) out_buf[out_x] = '0' + (num % 10);
                        for (int t = out_x - 1; s < t; ++s, --t) swap(out_buf[s], out_buf[t]);
                        if (out_x >= K / 2) flush();
                }
};

