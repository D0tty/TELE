FROM archlinux AS builder
WORKDIR /app
RUN echo 'Server = http://archlinux.polymorf.fr/$repo/os/$arch' > /etc/pacman.d/mirrorlist
RUN pacman -Suy --noconfirm
RUN pacman -S --noconfirm --needed \
              cmake \
              base-devel \
              boost \
              boost-libs \
              qt5-base
COPY ./CMakeLists.txt .
COPY include ./include
COPY src ./src
RUN mkdir build && cd build && cmake .. && make -j

FROM archlinux
WORKDIR /app
COPY --from=builder /app/build/tele .
COPY images ./images
RUN echo 'Server = http://archlinux.polymorf.fr/$repo/os/$arch' > /etc/pacman.d/mirrorlist
RUN pacman -Suy --noconfirm
RUN pacman -S --noconfirm --needed \
              boost-libs \
              qt5-base \
              qt5-webengine
RUN pacman -Sc
CMD ["/app/tele"]
