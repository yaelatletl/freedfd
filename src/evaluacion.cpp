/*
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <dfd.h>
#include <token.h>
#include <funcion.h>
#include <errores.h>
#include <arreglos.h>
#include <evaluacion.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <entorno-ejecucion.h>

Token *
EvaluaPostfijo (Token * Expresion)
{

  Token *Pila = 0;
  Token *Actual = Expresion;
  while (Actual)
    {

      switch (Actual->GetTipoToken ())
        {

        case OPERANDO:
          {
            Token *Nuevo = new Token (*Actual);
            Apila (&Pila, Nuevo);
            break;
          }

        case OPERADOR:

          char *Cadena = Actual->GetOperador ();
          Buzon.SetIdentificadorAsociado (Cadena);
          switch (Actual->GetAlcanceOperador ())
            {
            case ARRAY:
            case UNITARIOBINARIO: assert(0); break;

            case UNITARIO:
              if (!Pila)
                {
                  Buzon.SetIdentificadorAsociado ("2X");
                  Buzon.Error (ERROR_INDEFINIDO);
                  LiberarListaToken (Pila);
                  return 0;
                }
              /* ****************** */

              if (*Cadena == '$')
                AccesoArreglo (&Pila, atoi (Cadena + 1));

              else if (!strcmp (Cadena, "+"))
                ;
              else if (!strcmp (Cadena, "-"))
                MenosUnitario (&Pila);
              else if (!strcasecmp (Cadena, "SIN"))
                Seno (&Pila);
              else if (!strcasecmp (Cadena, "COS"))
                Coseno (&Pila);
              else if (!strcasecmp (Cadena, "TAN"))
                Tangente (&Pila);
              else if (!strcasecmp (Cadena, "SINH"))
                SenoHiperbolico (&Pila);
              else if (!strcasecmp (Cadena, "COSH"))
                CosenoHiperbolico (&Pila);
              else if (!strcasecmp (Cadena, "TANH"))
                TangenteHiperbolica (&Pila);
              else if (!strcasecmp (Cadena, "ASIN"))
                ArcoSeno (&Pila);
              else if (!strcasecmp (Cadena, "ACOS"))
                ArcoCoseno (&Pila);
              else if (!strcasecmp (Cadena, "ATAN"))
                ArcoTangente (&Pila);
              else if (!strcasecmp (Cadena, "EXP"))
                Exp (&Pila);
              else if (!strcasecmp (Cadena, "LOG"))
                Log (&Pila);
              else if (!strcasecmp (Cadena, "LN"))
                Ln (&Pila);
              else if (!strcasecmp (Cadena, "TRUNC"))
                Trunc (&Pila);
              else if (!strcasecmp (Cadena, "ROUND"))
                Round (&Pila);
              else if (!strcasecmp (Cadena, "ABS"))
                Abs (&Pila);
              else if (!strcasecmp (Cadena, "SQRT"))
                Sqrt (&Pila);
              else if (!strcasecmp (Cadena, "NOT"))
                Not (&Pila);
              else if (!strcasecmp (Cadena, "RANDOM"))
                Random (&Pila);
              else if (!strcasecmp (Cadena, "LEN"))
                Len (&Pila);
              else if (*Cadena == '@')
                Substring (&Pila, atoi (Cadena + 1));
              else
                {
                  Buzon.SetIdentificadorAsociado ("3X");
                  Buzon.Error (ERROR_INDEFINIDO);
                  LiberarListaToken (Pila);
                  return 0;
                }
              break;

            case BINARIO:
              {
                bool Error = true;
                if (Pila)
                  if (Pila->GetSig ())
                    Error = false;
                if (Error)
                  {
                    Buzon.SetIdentificadorAsociado ("1X");
                    Buzon.Error (ERROR_INDEFINIDO);
                    LiberarListaToken (Pila);
                    return 0;
                  }
              }

              if (!strcmp (Cadena, "+"))
                Sumar (&Pila);
              else if (!strcmp (Cadena, "-"))
                Restar (&Pila);
              else if (!strcmp (Cadena, "*"))
                Multiplicar (&Pila);
              else if (!strcmp (Cadena, "/"))
                Dividir (&Pila);
              else if (!strcmp (Cadena, "^"))
                Elevar (&Pila);
              else if (!strcasecmp (Cadena, "AND"))
                And (&Pila);
              else if (!strcasecmp (Cadena, "OR"))
                Or (&Pila);
              else if (!strcasecmp (Cadena, "XOR"))
                Xor (&Pila);
              else if (!strcasecmp (Cadena, "NAND"))
                Nand (&Pila);
              else if (!strcasecmp (Cadena, "NOR"))
                Nor (&Pila);
              else if (!strcasecmp (Cadena, "XNOR"))
                Xnor (&Pila);
              else if (!strcasecmp (Cadena, "MOD"))
                Modulo (&Pila);
              else if (!strcmp (Cadena, "<"))
                Menor (&Pila);
              else if (!strcmp (Cadena, ">"))
                Mayor (&Pila);
              else if (!strcmp (Cadena, "="))
                Igual (&Pila);
              else if (!strcmp (Cadena, "!="))
                NoIgual (&Pila);
              else if (!strcmp (Cadena, "<="))
                MenorIgual (&Pila);
              else if (!strcmp (Cadena, ">="))
                MayorIgual (&Pila);
              else
                {
                  Buzon.SetIdentificadorAsociado ("4X");
                  Buzon.Error (ERROR_INDEFINIDO);
                  LiberarListaToken (Pila);
                  return 0;
                }
            }                   //switch
        }                       //switch
      if (Buzon.GetHuboError ())
        {
          LiberarListaToken (Pila);
          return 0;
        }
      Actual = Actual->GetSig ();
    }                           //while
  bool Error = false;
  if (Pila)
    {
      if (Pila->GetSig ())
        Error = true;
    }
  else
    Error = true;

  if (Error)
    {
      Buzon.SetIdentificadorAsociado ("5X");
      Buzon.Error (ERROR_INDEFINIDO);
      LiberarListaToken (Pila);
      return 0;
    }

  if (Pila->GetTipoAlmacenamiento () == CONSTANTE)
    return Pila;
  Token *TokenRetorno = EntornoEjecucion_BuscaSimbolo (Pila->GetDatoStr());
  delete Pila;
  return TokenRetorno;
}

