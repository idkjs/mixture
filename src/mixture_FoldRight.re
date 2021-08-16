/* Mixture_FoldRight --  Right foldable mixin

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
  let fold: (('a, 'b) => 'b, t('a), 'b) => 'b;
};

module type Methods = {
  type t(+'a);
  let iter: ('a => unit, t('a)) => unit;
  let elements: t('a) => list('a);
  let cardinal: t('a) => int;
  let exists: ('a => bool, t('a)) => bool;
  let for_all: ('a => bool, t('a)) => bool;
};

module Make = (B: Basis) => {
  let iter = (f, cont) => {
    let loop = (a, ()) => f(a);
    B.fold(loop, cont, ());
  };

  let elements = cont => {
    let loop = (a, acc) => [a, ...acc];
    B.fold(loop, cont, []);
  };

  let cardinal = cont => B.fold((_, acc) => succ(acc), cont, 0);

  let exists = (p, cont) => {
    let loop = a =>
      if (p(a)) {
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
    let loop = a =>
      if (p(a)) {
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
  include Basis with type t('a) := t('a);
  include Methods with type t('a) := t('a);
};
