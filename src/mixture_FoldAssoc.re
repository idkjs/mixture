/* Mixture_FoldAssoc -- Foldable associative mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

exception Mixture_Exit;
/* A private exception used by the for_all and exists functions. */

module type Basis = {
  type t(+'a);
  type key;
  let fold: ((key, 'a, 'b) => 'b, t('a), 'b) => 'b;
};

module type Methods = {
  type t(+'a);
  type key;
  let iter: ((key, 'a) => unit, t('a)) => unit;
  let elements: t('a) => list('a);
  let bindings: t('a) => list((key, 'a));
  let cardinal: t('a) => int;
  let exists: ((key, 'a) => bool, t('a)) => bool;
  let for_all: ((key, 'a) => bool, t('a)) => bool;
};

module Make = (B: Basis) => {
  let iter = (f, cont) => {
    let loop = (k, a, ()) => f(k, a);
    B.fold(loop, cont, ());
  };

  let elements = cont => {
    let loop = (_, a, acc) => [a, ...acc];
    B.fold(loop, cont, []);
  };

  let bindings = cont => {
    let loop = (k, a, acc) => [(k, a), ...acc];
    B.fold(loop, cont, []);
  };

  let cardinal = cont => B.fold((_, _, acc) => succ(acc), cont, 0);

  let exists = (p, cont) => {
    let loop = (k, a) =>
      if (p(k, a)) {
        raise(Mixture_Exit);
      } else {
        ();
      };

    try(
      {
        iter(loop, cont);
        false;
      }
    ) {
    | Mixture_Exit => true
    };
  };

  let for_all = (p, cont) => {
    let loop = (k, a) =>
      if (p(k, a)) {
        ();
      } else {
        raise(Mixture_Exit);
      };

    try(
      {
        iter(loop, cont);
        true;
      }
    ) {
    | Mixture_Exit => false
    };
  };
};

module type S = {
  type t(+'a);
  type key;
  include Basis with type t('a) := t('a) and type key := key;
  include Methods with type t('a) := t('a) and type key := key;
};
