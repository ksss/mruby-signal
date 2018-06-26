FROM alpine:3.6
RUN apk add --no-cache git gcc make bison ruby musl-dev
RUN git clone --depth 1 https://github.com/mruby/mruby.git
RUN git clone --depth 1 https://github.com/ksss/mruby-spec.git
RUN mkdir -p /mruby-spec/unsupported/signal
RUN echo "Signal.list doesn't contain other signals than the known list" > /mruby-spec/unsupported/signal/list.txt
